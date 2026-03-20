#include "CCharacter.h"

#include <CEngine.h>
#include <CDevice.h>
#include <numbers>
#include <Component/CColliderBox2D.h>
#include <World/CWorld.h>

#include "../Strings.h"
#include "../UI/CDamageFloatingText.h"

bool CCharacter::Init()
{
	if (!CGameObject::Init())
	{
		return false;
	}

	Root = CreateComponent<CSceneComponent>(Key::Comp::Root);

	Collider = CreateComponent<CColliderBox2D>(Key::Comp::Collider, Key::Comp::Root);
	if (auto Collider = this->Collider.lock())
	{
		Collider->SetDrawDebug(false);
		Collider->SetInheritScale(false);

		SleepOnSpawnComponents.push_back(Collider);
		Collider->SetEnable(false);
	}

	return true;
}

void CCharacter::Update(const float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	switch (SpawnState)
	{
		case ESpawnState::Spawning:
			UpdateSpawnSequence(DeltaTime);
			break;
		case ESpawnState::PendingNotify:
			OnSpawnFinished();
			break;
	}

	if (bIsSpiralShrinking)
	{
		auto Root = this->Root.lock();
		if (!Root)
		{
			return;
		}

		SpiralShrinkElapsed += DeltaTime;

		if (SpiralShrinkElapsed >= SpiralShrinkDuration)
		{
			Root->SetRelativeScale(0.f, 0.f);
			Destroy();
			return;
		}

		float Progress = SpiralShrinkElapsed / SpiralShrinkDuration;
		float Scale = 1.f - Progress;
		Root->SetRelativeScale(Scale, Scale);

		float RotationDelta = SpiralRotationSpeed * DeltaTime;
		float CurrentRotation = Root->GetRelativeRotation().z;
		Root->SetRelativeRotationZ(CurrentRotation + RotationDelta);
	}
	else
	{
		SquashAndStretch(DeltaTime, SSIntensity, SSPeriod);
	}
}

void CCharacter::Destroy()
{
	CGameObject::Destroy();
}

CCharacter* CCharacter::Clone()
{
	return new CCharacter(*this);
}

float CCharacter::TakeDamage(float Damage, const std::weak_ptr<CGameObject>& Instigator)
{
	const auto World = this->World.lock();
	const auto UIManager = World->GetUIManager().lock();
	const auto DamageWidget = UIManager->CreateWidget<CDamageFloatingText>("DamageFloating").lock();

	if (DamageWidget)
	{
		constexpr int RandomRangeX = 30;
		constexpr int RandomRangeY = 10;
		constexpr float VerticalScreenOffset = -10.f;

		auto& RandomEngine = CEngine::GetInst()->GetMT();
		std::uniform_int_distribution<int> RandomXDist(-RandomRangeX, RandomRangeX);
		std::uniform_int_distribution<int> RandomYDist(-RandomRangeY, RandomRangeY);

		const float RandomOffsetX = static_cast<float>(RandomXDist(RandomEngine));
		const float RandomOffsetY = static_cast<float>(RandomYDist(RandomEngine));

		FVector AnchorWorldPos = GetWorldPosition();
		AnchorWorldPos.y += GetWorldScale().y * 0.5f;

		DamageWidget->SetAnchorWorldPos(
			AnchorWorldPos,
			RandomOffsetX,
			RandomOffsetY + VerticalScreenOffset);
		DamageWidget->SetEnable(true);
		DamageWidget->SetDamage(Damage);
	}

	return CGameObject::TakeDamage(Damage, Instigator);
}

void CCharacter::OnDead()
{
	bIsPendingDead = true;

	if (auto Col = Collider.lock())
	{
		Col->SetEnable(false);
	}

	SpiralShrink();
}

void CCharacter::SpiralShrink(float Duration, float RotationSpeed)
{
	bIsSpiralShrinking = true;
	SpiralShrinkElapsed = 0.f;
	SpiralShrinkDuration = Duration;
	SpiralRotationSpeed = RotationSpeed;
}

void CCharacter::UpdateSpawnSequence(const float DeltaTime)
{
	ElapsedSpawnTime += DeltaTime;

	if (ElapsedSpawnTime >= TimeToSpawn)
	{
		SpawnState = ESpawnState::PendingNotify;
	}
}

void CCharacter::OnSpawnFinished()
{
	SpawnState = ESpawnState::Complete;

	auto SleepCompView = SleepOnSpawnComponents
		| std::views::transform([](const auto& Weak) { return Weak.lock(); })
		| std::views::filter([](const auto& Comp) { return Comp != nullptr && Comp->GetAlive(); });

	for (const auto& Comp : SleepCompView)
	{
		Comp->SetEnable(true);
	}
}

void CCharacter::SquashAndStretch(const float DeltaTime, float Intensity, float Period)
{
	auto Root = this->Root.lock();
	if (!Root)
	{
		return;
	}

	SSElapsed += DeltaTime;

	float Theta = SSElapsed * static_cast<float>(std::numbers::pi) * 2 / Period;
	float SinValue = std::sin(Theta);

	FVector2 NewScale;
	NewScale.x = 1 + (SinValue * Intensity);
	NewScale.y = 1 - (SinValue * Intensity);

	Root->SetRelativeScale(NewScale);
}

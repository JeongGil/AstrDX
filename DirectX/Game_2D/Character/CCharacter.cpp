#include "CCharacter.h"

#include <CEngine.h>
#include <numbers>
#include <Component/CColliderBox2D.h>
#include "../Strings.h"

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
	// TODO: Damage Floating Text.
	//const FVector Offset = { 0,20,0 };
	//const int RangeX = 100;
	//const int RangeY = 50;	

	//std::uniform_int_distribution<int> DistX(-RangeX, RangeX);
	//std::uniform_int_distribution<int> DistY(-RangeY, RangeY);
	//auto& RandEngine = CEngine::GetInst()->GetMT();

	//int X = DistX(RandEngine);
	//int Y = DistY(RandEngine);
	//const FVector SpawnPos = { Offset.x + X, Offset.y + Y,Offset.z };

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

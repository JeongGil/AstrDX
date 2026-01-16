#include "CCharacter.h"

#include "Component/CColliderBox2D.h"
#include "../Strings.h"
#include <numbers>

bool CCharacter::Init()
{
	if (!CGameObject::Init())
	{
		return false;
	}

	Root = CreateComponent<CSceneComponent>(Key::Comp::Root);

	Collider = CreateComponent<CColliderBox2D>(Key::Comp::Collider);
	if (auto Collider = this->Collider.lock())
	{
		Collider->SetCollisionProfile("Character");
		Collider->SetDrawDebug(false);
		Collider->SetInheritScale(false);
	}

	return true;
}

void CCharacter::Update(const float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	SquashAndStretch(DeltaTime, SSIntensity, SSPeriod);
}

void CCharacter::Destroy()
{
	CGameObject::Destroy();
}

CCharacter* CCharacter::Clone()
{
	return new CCharacter(*this);
}

void CCharacter::SquashAndStretch(float DeltaTime, float Intensity, float Period)
{
	auto Root = this->Root.lock();
	if (!Root)
	{
		return;
	}

	SSElapsed += DeltaTime;

	float Theta = SSElapsed * std::numbers::pi * 2 / Period;
	float SinValue = std::sin(Theta);

	FVector2 NewScale;
	NewScale.x = 1 + (SinValue * Intensity);
	NewScale.y = 1 - (SinValue * Intensity);

	Root->SetRelativeScale(NewScale);
}

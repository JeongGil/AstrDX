#include "CObjectMovementComponent.h"

#include "CSceneComponent.h"

bool CObjectMovementComponent::Init()
{
	if (!CMovementComponent::Init())
	{
		return false;
	}

	return true;
}

void CObjectMovementComponent::Update(const float Delta)
{
	CMovementComponent::Update(Delta);
}

void CObjectMovementComponent::PostUpdate(const float DeltaTime)
{
	CMovementComponent::PostUpdate(DeltaTime);

	auto Cmp = UpdateComponent.lock();
	if (!Cmp)
	{
		Velocity = FVector::Zero;
		return;
	}

	if (MoveDirection.IsZero())
	{
		Velocity = FVector::Zero;
		return;
	}

	MoveDirection.Normalize();

	Velocity = MoveDirection * (Speed * DeltaTime);
	Cmp->AddWorldPosition(Velocity);

	MoveDirection = FVector::Zero;
}

void CObjectMovementComponent::Destroy()
{
	CMovementComponent::Destroy();
}

CObjectMovementComponent* CObjectMovementComponent::Clone() const
{
	return new CObjectMovementComponent(*this);
}

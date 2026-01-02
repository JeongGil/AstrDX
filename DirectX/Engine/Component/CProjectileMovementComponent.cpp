#include "CProjectileMovementComponent.h"

#include "CSceneComponent.h"

bool CProjectileMovementComponent::Init()
{
	if (!CMovementComponent::Init())
	{
		return false;
	}

	return true;
}

void CProjectileMovementComponent::Update(const float Delta)
{
	CMovementComponent::Update(Delta);
}

void CProjectileMovementComponent::PostUpdate(const float DeltaTime)
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

	if (Range > 0.f)
	{
		Range -= Speed * DeltaTime;

		if (Range <= 0.f)
		{
			if (RangeFunction)
			{
				RangeFunction();
			}

			MoveDirection = FVector::Zero;
		}
	}
}

void CProjectileMovementComponent::Destroy()
{
	CMovementComponent::Destroy();
}

CProjectileMovementComponent* CProjectileMovementComponent::Clone() const
{
	return new CProjectileMovementComponent(*this);
}

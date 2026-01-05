#include "CRotationMovementComponent.h"

#include "CSceneComponent.h"

bool CRotationMovementComponent::Init()
{
	if (!CMovementComponent::Init())
	{
		return false;
	}

	return true;
}

void CRotationMovementComponent::Update(const float Delta)
{
	CMovementComponent::Update(Delta);
}

void CRotationMovementComponent::PostUpdate(const float DeltaTime)
{
	CMovementComponent::PostUpdate(DeltaTime);

	if (auto Cmp = UpdateComponent.lock())
	{
		auto Angle = AngularSpeedInDegree * DeltaTime;
		switch (Axis)
		{
		case EAxis::X:
			Cmp->AddRelativeRotationX(Angle);
			break;
		case EAxis::Y:
			Cmp->AddRelativeRotationY(Angle);
			break;
		case EAxis::Z:
			Cmp->AddRelativeRotationZ(Angle);
			break;
		default:
			break;
		}
	}
}

void CRotationMovementComponent::Destroy()
{
	CMovementComponent::Destroy();
}

CRotationMovementComponent* CRotationMovementComponent::Clone() const
{
	return new CRotationMovementComponent(*this);
}

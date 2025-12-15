#include "CSceneComponent.h"

void CSceneComponent::SetInheritScale(bool bInherit)
{
	bInheritScale = bInherit;
}

void CSceneComponent::SetInheritRot(bool bInherit)
{
	bInheritRotation = bInherit;
}

void CSceneComponent::SetRelativeScale(const FVector3& Scale)
{
	RelativeScale = Scale;

	WorldScale = RelativeScale;
	if (bInheritScale)
	{
		if (const auto Parent = this->Parent.lock())
		{
			WorldScale *= Parent->WorldScale;
		}
	}

	for (auto& Child : Children)
	{

	}
}

void CSceneComponent::SetRelativeScale(const FVector2& Scale)
{
	SetRelativeScale(FVector(Scale.x, Scale.y, RelativeScale.z));
}

void CSceneComponent::SetRelativeScale(float X, float Y, float Z)
{
	SetRelativeScale(FVector(X, Y, Z));
}

void CSceneComponent::SetRelativeScale(float X, float Y)
{
	SetRelativeScale(FVector(X, Y, RelativeScale.z));
}

void CSceneComponent::SetRelativeRotation(const FVector3& Rotation)
{
	RelativeRotation = Rotation;

	WorldRotation = RelativeRotation;
	if (bInheritRotation)
	{
		if (const auto Parent = this->Parent.lock())
		{
			WorldRotation += Parent->WorldRotation;
		}
	}

	// Also rotate axis.
	FMatrix RotationMatrix;
	RotationMatrix.Rotation(WorldRotation);

	for (int i = 0; i < EAxis::End; i++)
	{
		WorldAxis[i] = FVector::Axis[i].TransformNormal(RotationMatrix);
		WorldAxis[i].Normalize();
	}

	for (auto& Child : Children)
	{
		
	}
}

void CSceneComponent::SetRelativeRotation(const FVector2& Rotation)
{
	SetRelativeRotation(FVector(Rotation.x, Rotation.y, RelativeRotation.z));
}

void CSceneComponent::SetRelativeRotation(float X, float Y, float Z)
{
	SetRelativeRotation(FVector(X, Y, Z));
}

void CSceneComponent::SetRelativeRotation(float X, float Y)
{
	SetRelativeRotation(FVector(X, Y, RelativeRotation.z));
}

void CSceneComponent::SetRelativeRotationX(float X)
{
	SetRelativeRotation(FVector(X, RelativeRotation.y, RelativeRotation.z));
}

void CSceneComponent::SetRelativeRotationY(float Y)
{
	SetRelativeRotation(FVector(RelativeRotation.x, Y, RelativeRotation.z));
}

void CSceneComponent::SetRelativeRotationZ(float Z)
{
	SetRelativeRotation(FVector(RelativeRotation.x, RelativeRotation.y, Z));
}

void CSceneComponent::SetRelativePosition(const FVector3& Position)
{
	RelativePosition = Position;
}

void CSceneComponent::SetRelativePosition(const FVector2& Position)
{
	SetRelativePosition(FVector(Position.x, Position.y, RelativePosition.z));
}

void CSceneComponent::SetRelativePosition(float X, float Y, float Z)
{
	SetRelativePosition(FVector(X, Y, Z));
}

void CSceneComponent::SetRelativePosition(float X, float Y)
{
	SetRelativePosition(FVector(X, Y, RelativePosition.z));
}

bool CSceneComponent::Init()
{
	return true;
}

void CSceneComponent::Update(const float Delta)
{
}

void CSceneComponent::Render()
{
}

CComponent* CSceneComponent::Clone() const
{
	return new CSceneComponent(*this);
}

void CSceneComponent::Destroy()
{
	bAlive = false;
}

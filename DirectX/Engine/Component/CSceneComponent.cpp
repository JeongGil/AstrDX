#include "CSceneComponent.h"

#include "../Render/CRenderManager.h"

void CSceneComponent::AddChild(const std::weak_ptr<CSceneComponent>& Child)
{
	if (auto Shared = Child.lock())
	{
		Shared->Parent = std::dynamic_pointer_cast<CSceneComponent>(shared_from_this());
		Children.push_back(Child);

		Shared->SetRelativeScale(RelativeScale);
		Shared->SetRelativeRotation(RelativeRotation);
		Shared->SetRelativePosition(RelativePosition);
	}
}

void CSceneComponent::UpdateTransform()
{
	ScaleMatrix.Scaling(WorldScale);
	RotationMatrix.Rotation(WorldRotation);
	TranslateMatrix.Translation(WorldPosition);

	WorldMatrix = ScaleMatrix * RotationMatrix * TranslateMatrix;
}

const FVector& CSceneComponent::GetAxis(EAxis::Type Axis) const
{
	return WorldAxis[Axis];
}

const FVector& CSceneComponent::GetRelativeScale() const
{
	return RelativeScale;
}

const FVector& CSceneComponent::GetRelativeRotation() const
{
	return RelativeRotation;
}

const FVector& CSceneComponent::GetRelativePosition() const
{
	return RelativePosition;
}

const FVector& CSceneComponent::GetWorldScale() const
{
	return WorldScale;
}

const FVector& CSceneComponent::GetWorldRotation() const
{
	return WorldRotation;
}

const FVector& CSceneComponent::GetWorldPosition() const
{
	return WorldPosition;
}

void CSceneComponent::SetInheritScale(bool bInherit)
{
	bInheritScale = bInherit;
}

void CSceneComponent::SetInheritRotation(bool bInherit)
{
	bInheritRotation = bInherit;
}

void CSceneComponent::SetRelativeScale(const FVector& Scale)
{
	RelativeScale = Scale;

	InheritRelativeScale();
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

void CSceneComponent::AddRelativeScale(const FVector& Scale)
{
	SetRelativeScale(RelativeScale + Scale);
}

void CSceneComponent::AddRelativeScale(const FVector2& Scale)
{
	AddRelativeScale(FVector(Scale.x, Scale.y, 0.f));
}

void CSceneComponent::AddRelativeScale(float X, float Y, float Z)
{
	AddRelativeScale(FVector(X, Y, Z));
}

void CSceneComponent::AddRelativeScale(float X, float Y)
{
	AddRelativeScale(FVector(X, Y, 0.f));
}

void CSceneComponent::SetRelativeRotation(const FVector& Rotation)
{
	RelativeRotation = Rotation;

	InheritRelativeRotation();
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

void CSceneComponent::AddRelativeRotation(const FVector& Rotation)
{
	SetRelativeRotation(RelativeRotation + Rotation);
}

void CSceneComponent::AddRelativeRotation(const FVector2& Rotation)
{
	AddRelativeRotation(FVector(Rotation.x, Rotation.y, 0.f));
}

void CSceneComponent::AddRelativeRotation(float X, float Y, float Z)
{
	AddRelativeRotation(FVector(X, Y, Z));
}

void CSceneComponent::AddRelativeRotation(float X, float Y)
{
	AddRelativeRotation(FVector(X, Y, 0.f));
}

void CSceneComponent::AddRelativeRotationX(float X)
{
	AddRelativeRotation(FVector(X, 0.f, 0.f));
}

void CSceneComponent::AddRelativeRotationY(float Y)
{
	AddRelativeRotation(FVector(0.f, Y, 0.f));
}

void CSceneComponent::AddRelativeRotationZ(float Z)
{
	AddRelativeRotation(FVector(0.f, 0.f, Z));
}

void CSceneComponent::SetRelativePosition(const FVector& Position)
{
	RelativePosition = Position;

	InheritRelativePosition();
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

void CSceneComponent::AddRelativePosition(const FVector& Position)
{
	SetRelativePosition(RelativePosition + Position);
}

void CSceneComponent::AddRelativePosition(const FVector2& Position)
{
	AddRelativePosition(FVector(Position.x, Position.y, 0.f));
}

void CSceneComponent::AddRelativePosition(float X, float Y, float Z)
{
	AddRelativePosition(FVector(X, Y, Z));
}

void CSceneComponent::AddRelativePosition(float X, float Y)
{
	AddRelativePosition(FVector(X, Y, 0.f));
}

void CSceneComponent::InheritRelativeScale()
{
	WorldScale = RelativeScale;
	if (bInheritScale)
	{
		if (auto Parent = this->Parent.lock())
		{
			WorldScale *= Parent->WorldScale;
		}
	}

	for (const auto& WeakChild : Children)
	{
		if (auto Child = WeakChild.lock())
		{
			Child->InheritRelativeScale();
		}
	}
}

void CSceneComponent::InheritRelativeRotation()
{
	WorldRotation = RelativeRotation;
	if (bInheritRotation)
	{
		if (auto Parent = this->Parent.lock())
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

	for (const auto& WeakChild : Children)
	{
		if (auto Child = WeakChild.lock())
		{
			Child->InheritRelativeRotation();
		}
	}

	for (const auto& WeakChild : Children)
	{
		if (auto Child = WeakChild.lock())
		{
			Child->InheritRelativePosition();
		}
	}
}

void CSceneComponent::InheritRelativePosition()
{
	if (auto Parent = this->Parent.lock())
	{
		FMatrix ParentMatrix;
		if (bInheritRotation)
		{
			FVector ParentRotation = Parent->GetWorldRotation();
			ParentMatrix.Rotation(ParentRotation);
		}

		FVector ParentPosition = Parent->GetWorldPosition();
		// _41, _42, _43 are the translation components.
		memcpy(&ParentMatrix._41, &ParentPosition, sizeof(FVector));

		WorldPosition = RelativePosition.TransformCoord(ParentMatrix);
	}
	else
	{
		WorldPosition = RelativePosition;
	}

	for (const auto& WeakChild : Children)
	{
		if (auto Child = WeakChild.lock())
		{
			Child->InheritRelativePosition();
		}
	}
}

void CSceneComponent::SetWorldScale(const FVector& Scale)
{
	WorldScale = Scale;

	InheritWorldScale();
}

void CSceneComponent::SetWorldScale(const FVector2& Scale)
{
	SetWorldScale(FVector(Scale.x, Scale.y, WorldScale.z));
}

void CSceneComponent::SetWorldScale(float X, float Y, float Z)
{
	SetWorldScale(FVector(X, Y, Z));
}

void CSceneComponent::SetWorldScale(float X, float Y)
{
	SetWorldScale(FVector(X, Y, WorldScale.z));
}

void CSceneComponent::AddWorldScale(const FVector& Scale)
{
	SetWorldScale(WorldScale + Scale);
}

void CSceneComponent::AddWorldScale(const FVector2& Scale)
{
	AddWorldScale(FVector(Scale.x, Scale.y, 0.f));
}

void CSceneComponent::AddWorldScale(float X, float Y, float Z)
{
	AddWorldScale(FVector(X, Y, Z));
}

void CSceneComponent::AddWorldScale(float X, float Y)
{
	AddWorldScale(FVector(X, Y, 0.f));
}

void CSceneComponent::SetWorldRotation(const FVector& Rotation)
{
	WorldRotation = Rotation;

	InheritWorldRotation();
}

void CSceneComponent::SetWorldRotation(const FVector2& Rotation)
{
	SetWorldRotation(FVector(Rotation.x, Rotation.y, WorldRotation.z));
}

void CSceneComponent::SetWorldRotation(float X, float Y, float Z)
{
	SetWorldRotation(FVector(X, Y, Z));
}

void CSceneComponent::SetWorldRotation(float X, float Y)
{
	SetWorldRotation(FVector(X, Y, WorldRotation.z));
}

void CSceneComponent::SetWorldRotationX(float X)
{
	SetWorldRotation(FVector(X, WorldRotation.y, WorldRotation.z));
}

void CSceneComponent::SetWorldRotationY(float Y)
{
	SetWorldRotation(FVector(WorldRotation.x, Y, WorldRotation.z));
}

void CSceneComponent::SetWorldRotationZ(float Z)
{
	SetWorldRotation(FVector(WorldRotation.x, WorldRotation.y, Z));
}

void CSceneComponent::AddWorldRotation(const FVector& Rotation)
{
	SetWorldRotation(WorldRotation + Rotation);
}

void CSceneComponent::AddWorldRotation(const FVector2& Rotation)
{
	AddWorldRotation(FVector(Rotation.x, Rotation.y, 0.f));
}

void CSceneComponent::AddWorldRotation(float X, float Y, float Z)
{
	AddWorldRotation(FVector(X, Y, Z));
}

void CSceneComponent::AddWorldRotation(float X, float Y)
{
	AddWorldRotation(FVector(X, Y, 0.f));
}

void CSceneComponent::AddWorldRotationX(float X)
{
	AddWorldRotation(FVector(X, 0.f, 0.f));
}

void CSceneComponent::AddWorldRotationY(float Y)
{
	AddWorldRotation(FVector(0.f, Y, 0.f));
}

void CSceneComponent::AddWorldRotationZ(float Z)
{
	AddWorldRotation(FVector(0.f, 0.f, Z));
}

void CSceneComponent::SetWorldPosition(const FVector& Position)
{
	UpdateChildWorldPosition(Position - WorldPosition);

	InheritWorldPosition();
}

void CSceneComponent::SetWorldPosition(const FVector2& Position)
{
	SetWorldPosition(FVector(Position.x, Position.y, WorldPosition.z));
}

void CSceneComponent::SetWorldPosition(float X, float Y, float Z)
{
	SetWorldPosition(FVector(X, Y, Z));
}

void CSceneComponent::SetWorldPosition(float X, float Y)
{
	SetWorldPosition(FVector(X, Y, WorldPosition.z));
}

void CSceneComponent::AddWorldPosition(const FVector& Position)
{
	SetWorldPosition(WorldPosition + Position);
}

void CSceneComponent::AddWorldPosition(const FVector2& Position)
{
	AddWorldPosition(FVector(Position.x, Position.y, 0.f));
}

void CSceneComponent::AddWorldPosition(float X, float Y, float Z)
{
	AddWorldPosition(FVector(X, Y, Z));
}

void CSceneComponent::AddWorldPosition(float X, float Y)
{
	AddWorldPosition(FVector(X, Y, 0.f));
}

void CSceneComponent::UpdateChildWorldPosition(const FVector& DeltaPosition)
{
	WorldPosition += DeltaPosition;

	for (const auto& WeakChild : Children)
	{
		if (auto Child = WeakChild.lock())
		{
			Child->UpdateChildWorldPosition(DeltaPosition);
		}
	}
}

void CSceneComponent::InheritWorldScale()
{
	RelativeScale = WorldScale;
	if (bInheritScale)
	{
		if (auto Parent = this->Parent.lock())
		{
			RelativeScale /= Parent->GetWorldScale();
		}
	}

	for (const auto& WeakChild : Children)
	{
		if (auto Child = WeakChild.lock())
		{
			Child->InheritWorldScale();
		}
	}
}

void CSceneComponent::InheritWorldRotation()
{
	RelativeRotation = WorldRotation;
	if (bInheritRotation)
	{
		if (auto Parent = this->Parent.lock())
		{
			WorldRotation -= Parent->WorldRotation;
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

	for (const auto& WeakChild : Children)
	{
		if (auto Child = WeakChild.lock())
		{
			Child->InheritWorldRotation();
		}
	}

	for (const auto& WeakChild : Children)
	{
		if (auto Child = WeakChild.lock())
		{
			Child->InheritWorldPosition();
		}
	}
}

void CSceneComponent::InheritWorldPosition()
{
	if (auto Parent = this->Parent.lock())
	{
		FMatrix ParentMatrix;
		if (bInheritRotation)
		{
			FVector ParentRotation = Parent->GetWorldRotation();
			ParentMatrix.Rotation(ParentRotation);
		}

		FVector ParentPosition = Parent->GetWorldPosition();
		// _41, _42, _43 are the translation components.
		memcpy(&ParentMatrix._41, &ParentPosition, sizeof(FVector));

		RelativePosition = WorldPosition.TransformCoord(ParentMatrix);
	}
	else
	{
		RelativePosition = WorldPosition;
	}

	for (const auto& WeakChild : Children)
	{
		if (auto Child = WeakChild.lock())
		{
			Child->InheritWorldPosition();
		}
	}
}

bool CSceneComponent::Init()
{
	if (!CComponent::Init())
	{
		return false;
	}

	if (RenderType == EComponentRender::Render)
	{
		CRenderManager::GetInst()->AddRenderLayer(std::dynamic_pointer_cast<CSceneComponent>(shared_from_this()));
	}

	return true;
}

void CSceneComponent::Update(const float DeltaTime)
{
	for (const auto& WeakChild : Children)
	{
		if (auto Child = WeakChild.lock())
		{
			Child->Update(DeltaTime);
		}
	}
}

void CSceneComponent::PostUpdate(const float DeltaTime)
{
	UpdateTransform();

	for (const auto& WeakChild : Children)
	{
		if (auto Child = WeakChild.lock())
		{
			Child->PostUpdate(DeltaTime);
		}
	}
}

void CSceneComponent::Render()
{
	// All components that need to be rendered are rendered by the RenderManager.
	//for (const auto& WeakChild : Children)
	//{
	//	if (auto Child = WeakChild.lock())
	//	{
	//		Child->Render();
	//	}
	//}
}

CSceneComponent* CSceneComponent::Clone() const
{
	return new CSceneComponent(*this);
}

void CSceneComponent::Destroy()
{
	CComponent::Destroy();
}

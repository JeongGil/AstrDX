#include "CGameObject.h"

void CGameObject::SetWorld(const std::weak_ptr<CWorld>& World)
{
	this->World = World;

	for (const auto& Cmp : SceneComponents)
	{
		Cmp->SetWorld(World);
	}
}

const FVector& CGameObject::GetAxis(EAxis::Type Axis) const
{
	if (auto Root = this->Root.lock())
	{
		return Root->GetAxis(Axis);
	}

	return FVector::Zero;
}

const FVector& CGameObject::GetRelativeScale() const
{
	if (auto Root = this->Root.lock())
	{
		return Root->GetRelativeScale();
	}

	return FVector::Zero;
}

const FVector& CGameObject::GetRelativeRotation() const
{
	if (auto Root = this->Root.lock())
	{
		return Root->GetRelativeRotation();
	}

	return FVector::Zero;
}

const FVector& CGameObject::GetRelativePosition() const
{
	if (auto Root = this->Root.lock())
	{
		return Root->GetRelativePosition();
	}

	return FVector::Zero;
}

const FVector& CGameObject::GetWorldScale() const
{
	if (auto Root = this->Root.lock())
	{
		return Root->GetWorldScale();
	}

	return FVector::Zero;
}

const FVector& CGameObject::GetWorldRotation() const
{
	if (auto Root = this->Root.lock())
	{
		return Root->GetWorldRotation();
	}

	return FVector::Zero;
}

const FVector& CGameObject::GetWorldPosition() const
{
	if (auto Root = this->Root.lock())
	{
		return Root->GetWorldPosition();
	}

	return FVector::Zero;
}

void CGameObject::SetInheritScale(bool bInherit) const
{
	if (auto Root = this->Root.lock())
	{
		Root->SetInheritScale(bInherit);
	}
}

void CGameObject::SetInheritRotation(bool bInherit) const
{
	if (auto Root = this->Root.lock())
	{
		Root->SetInheritRotation(bInherit);
	}
}

void CGameObject::SetRelativeScale(const FVector3& Scale) const
{
	if (auto Root = this->Root.lock())
	{
		Root->SetRelativeScale(Scale);
	}
}

void CGameObject::SetRelativeScale(const FVector2& Scale) const
{
	if (auto Root = this->Root.lock())
	{
		Root->SetRelativeScale(Scale);
	}
}

void CGameObject::SetRelativeScale(float X, float Y, float Z) const
{
	if (auto Root = this->Root.lock())
	{
		Root->SetRelativeScale(X, Y, Z);
	}
}

void CGameObject::SetRelativeScale(float X, float Y) const
{
	if (auto Root = this->Root.lock())
	{
		Root->SetRelativeScale(X, Y);
	}
}

void CGameObject::AddRelativeScale(const FVector3& Scale) const
{
	if (auto Root = this->Root.lock())
	{
		Root->AddRelativeScale(Scale);
	}
}

void CGameObject::AddRelativeScale(const FVector2& Scale) const
{
	if (auto Root = this->Root.lock())
	{
		Root->AddRelativeScale(Scale);
	}
}

void CGameObject::AddRelativeScale(float X, float Y, float Z) const
{
	if (auto Root = this->Root.lock())
	{
		Root->AddRelativeScale(X, Y, Z);
	}
}

void CGameObject::AddRelativeScale(float X, float Y) const
{
	if (auto Root = this->Root.lock())
	{
		Root->AddRelativeScale(X, Y);
	}
}

void CGameObject::SetRelativeRotation(const FVector3& Rotation) const
{
	if (auto Root = this->Root.lock())
	{
		Root->SetRelativeRotation(Rotation);
	}
}

void CGameObject::SetRelativeRotation(const FVector2& Rotation) const
{
	if (auto Root = this->Root.lock())
	{
		Root->SetRelativeRotation(Rotation);
	}
}

void CGameObject::SetRelativeRotation(float X, float Y, float Z) const
{
	if (auto Root = this->Root.lock())
	{
		Root->SetRelativeRotation(X, Y, Z);
	}
}

void CGameObject::SetRelativeRotation(float X, float Y) const
{
	if (auto Root = this->Root.lock())
	{
		Root->SetRelativeRotation(X, Y);
	}
}

void CGameObject::SetRelativeRotationX(float X) const
{
	if (auto Root = this->Root.lock())
	{
		Root->SetRelativeRotationX(X);
	}
}

void CGameObject::SetRelativeRotationY(float Y) const
{
	if (auto Root = this->Root.lock())
	{
		Root->SetRelativeRotationY(Y);
	}
}

void CGameObject::SetRelativeRotationZ(float Z) const
{
	if (auto Root = this->Root.lock())
	{
		Root->SetRelativeRotationZ(Z);
	}
}

void CGameObject::AddRelativeRotation(const FVector3& Rotation) const
{
	if (auto Root = this->Root.lock())
	{
		Root->AddRelativeRotation(Rotation);
	}
}

void CGameObject::AddRelativeRotation(const FVector2& Rotation) const
{
	if (auto Root = this->Root.lock())
	{
		Root->AddRelativeRotation(Rotation);
	}
}

void CGameObject::AddRelativeRotation(float X, float Y, float Z) const
{
	if (auto Root = this->Root.lock())
	{
		Root->AddRelativeRotation(X, Y, Z);
	}
}

void CGameObject::AddRelativeRotation(float X, float Y) const
{
	if (auto Root = this->Root.lock())
	{
		Root->AddRelativeRotation(X, Y);
	}
}

void CGameObject::AddRelativeRotationX(float X) const
{
	if (auto Root = this->Root.lock())
	{
		Root->AddRelativeRotationX(X);
	}
}

void CGameObject::AddRelativeRotationY(float Y) const
{
	if (auto Root = this->Root.lock())
	{
		Root->AddRelativeRotationY(Y);
	}
}

void CGameObject::AddRelativeRotationZ(float Z) const
{
	if (auto Root = this->Root.lock())
	{
		Root->AddRelativeRotationZ(Z);
	}
}

void CGameObject::SetRelativePosition(const FVector3& Position) const
{
	if (auto Root = this->Root.lock())
	{
		Root->SetRelativePosition(Position);
	}
}

void CGameObject::SetRelativePosition(const FVector2& Position) const
{
	if (auto Root = this->Root.lock())
	{
		Root->SetRelativePosition(Position);
	}
}

void CGameObject::SetRelativePosition(float X, float Y, float Z) const
{
	if (auto Root = this->Root.lock())
	{
		Root->SetRelativePosition(X, Y, Z);
	}
}

void CGameObject::SetRelativePosition(float X, float Y) const
{
	if (auto Root = this->Root.lock())
	{
		Root->SetRelativePosition(X, Y);
	}
}

void CGameObject::AddRelativePosition(const FVector3& Position) const
{
	if (auto Root = this->Root.lock())
	{
		Root->AddRelativePosition(Position);
	}
}

void CGameObject::AddRelativePosition(const FVector2& Position) const
{
	if (auto Root = this->Root.lock())
	{
		Root->AddRelativePosition(Position);
	}
}

void CGameObject::AddRelativePosition(float X, float Y, float Z) const
{
	if (auto Root = this->Root.lock())
	{
		Root->AddRelativePosition(X, Y, Z);
	}
}

void CGameObject::AddRelativePosition(float X, float Y) const
{
	if (auto Root = this->Root.lock())
	{
		Root->AddRelativePosition(X, Y);
	}
}

void CGameObject::SetWorldScale(const FVector3& Scale) const
{
	if (auto Root = this->Root.lock())
	{
		Root->SetWorldScale(Scale);
	}
}

void CGameObject::SetWorldScale(const FVector2& Scale) const
{
	if (auto Root = this->Root.lock())
	{
		Root->SetWorldScale(Scale);
	}
}

void CGameObject::SetWorldScale(float X, float Y, float Z) const
{
	if (auto Root = this->Root.lock())
	{
		Root->SetWorldScale(X, Y, Z);
	}
}

void CGameObject::SetWorldScale(float X, float Y) const
{
	if (auto Root = this->Root.lock())
	{
		Root->SetWorldScale(X, Y);
	}
}

void CGameObject::AddWorldScale(const FVector3& Scale) const
{
	if (auto Root = this->Root.lock())
	{
		Root->AddWorldScale(Scale);
	}
}

void CGameObject::AddWorldScale(const FVector2& Scale) const
{
	if (auto Root = this->Root.lock())
	{
		Root->AddWorldScale(Scale);
	}
}

void CGameObject::AddWorldScale(float X, float Y, float Z) const
{
	if (auto Root = this->Root.lock())
	{
		Root->AddWorldScale(X, Y, Z);
	}
}

void CGameObject::AddWorldScale(float X, float Y) const
{
	if (auto Root = this->Root.lock())
	{
		Root->AddWorldScale(X, Y);
	}
}

void CGameObject::SetWorldRotation(const FVector3& Rotation) const
{
	if (auto Root = this->Root.lock())
	{
		Root->SetWorldRotation(Rotation);
	}
}

void CGameObject::SetWorldRotation(const FVector2& Rotation) const
{
	if (auto Root = this->Root.lock())
	{
		Root->SetWorldRotation(Rotation);
	}
}

void CGameObject::SetWorldRotation(float X, float Y, float Z) const
{
	if (auto Root = this->Root.lock())
	{
		Root->SetWorldRotation(X, Y, Z);
	}
}

void CGameObject::SetWorldRotation(float X, float Y) const
{
	if (auto Root = this->Root.lock())
	{
		Root->SetWorldRotation(X, Y);
	}
}

void CGameObject::SetWorldRotationX(float X) const
{
	if (auto Root = this->Root.lock())
	{
		Root->SetWorldRotationX(X);
	}
}

void CGameObject::SetWorldRotationY(float Y) const
{
	if (auto Root = this->Root.lock())
	{
		Root->SetWorldRotationY(Y);
	}
}

void CGameObject::SetWorldRotationZ(float Z) const
{
	if (auto Root = this->Root.lock())
	{
		Root->SetWorldRotationZ(Z);
	}
}

void CGameObject::AddWorldRotation(const FVector3& Rotation) const
{
	if (auto Root = this->Root.lock())
	{
		Root->AddWorldRotation(Rotation);
	}
}

void CGameObject::AddWorldRotation(const FVector2& Rotation) const
{
	if (auto Root = this->Root.lock())
	{
		Root->AddWorldRotation(Rotation);
	}
}

void CGameObject::AddWorldRotation(float X, float Y, float Z) const
{
	if (auto Root = this->Root.lock())
	{
		Root->AddWorldRotation(X, Y, Z);
	}
}

void CGameObject::AddWorldRotation(float X, float Y) const
{
	if (auto Root = this->Root.lock())
	{
		Root->AddWorldRotation(X, Y);
	}
}

void CGameObject::AddWorldRotationX(float X) const
{
	if (auto Root = this->Root.lock())
	{
		Root->AddWorldRotationX(X);
	}
}

void CGameObject::AddWorldRotationY(float Y) const
{
	if (auto Root = this->Root.lock())
	{
		Root->AddWorldRotationY(Y);
	}
}

void CGameObject::AddWorldRotationZ(float Z) const
{
	if (auto Root = this->Root.lock())
	{
		Root->AddWorldRotationZ(Z);
	}
}

void CGameObject::SetWorldPosition(const FVector3& Position) const
{
	if (auto Root = this->Root.lock())
	{
		Root->SetWorldPosition(Position);
	}
}

void CGameObject::SetWorldPosition(const FVector2& Position) const
{
	if (auto Root = this->Root.lock())
	{
		Root->SetWorldPosition(Position);
	}
}

void CGameObject::SetWorldPosition(float X, float Y, float Z) const
{
	if (auto Root = this->Root.lock())
	{
		Root->SetWorldPosition(X, Y, Z);
	}
}

void CGameObject::SetWorldPosition(float X, float Y) const
{
	if (auto Root = this->Root.lock())
	{
		Root->SetWorldPosition(X, Y);
	}
}

void CGameObject::AddWorldPosition(const FVector3& Position) const
{
	if (auto Root = this->Root.lock())
	{
		Root->AddWorldPosition(Position);
	}
}

void CGameObject::AddWorldPosition(const FVector2& Position) const
{
	if (auto Root = this->Root.lock())
	{
		Root->AddWorldPosition(Position);
	}
}

void CGameObject::AddWorldPosition(float X, float Y, float Z) const
{
	if (auto Root = this->Root.lock())
	{
		Root->AddWorldPosition(X, Y, Z);
	}
}

void CGameObject::AddWorldPosition(float X, float Y) const
{
	if (auto Root = this->Root.lock())
	{
		Root->AddWorldPosition(X, Y);
	}
}

bool CGameObject::Init()
{
	return true;
}

void CGameObject::Update(const float DeltaTime)
{
	if (auto Root = this->Root.lock())
	{
		Root->Update(DeltaTime);
	}
}

void CGameObject::PostUpdate(const float DeltaTime)
{
	if (auto Root = this->Root.lock())
	{
		Root->PostUpdate(DeltaTime);
	}
}

void CGameObject::Render()
{
	if (auto Root = this->Root.lock())
	{
		Root->Render();
	}
}

CGameObject* CGameObject::Clone()
{
	return new CGameObject(*this);
}

void CGameObject::Destroy()
{
	bAlive = false;
}

#include "CGameObject.h"

bool CGameObject::GetSimulatePhysics() const
{
	if (auto Root = this->Root.lock())
	{
		return Root->GetSimulatePhysics();
	}

	return false;
}

bool CGameObject::GetUseGravity() const
{
	if (auto Root = this->Root.lock())
	{
		return Root->GetUseGravity();
	}

	return false;
}

void CGameObject::SetSimulatePhysics(bool bSimulate)
{
	if (auto Root = this->Root.lock())
	{
		Root->SetSimulatePhysics(bSimulate);
	}
}

void CGameObject::SetUseGravity(bool bUse)
{
	if (auto Root = this->Root.lock())
	{
		Root->SetUseGravity(bUse);
	}
}

void CGameObject::AddForce(const FVector& Force)
{
	if (auto Root = this->Root.lock())
	{
		Root->AddForce(Force);
	}
}

void CGameObject::ClearPhysics()
{
	if (auto Root = this->Root.lock())
	{
		Root->ClearPhysics();
	}
}

void CGameObject::SetWorld(const std::weak_ptr<CWorld>& World)
{
	this->World = World;

	for (const auto& Cmp : SceneComponents)
	{
		Cmp->SetWorld(World);
	}
}

const FVector& CGameObject::GetPivot() const
{
	if (auto Root = this->Root.lock())
	{
		return Root->GetPivot();
	}

	return FVector::Zero;
}

const FVector& CGameObject::GetVelocity() const
{
	if (auto Root = this->Root.lock())
	{
		return Root->GetVelocity();
	}

	return FVector::Zero;
}

float CGameObject::GetSpeed() const
{
	if (auto Root = this->Root.lock())
	{
		return Root->GetSpeed();
	}

	return 0.f;
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

void CGameObject::Begin()
{
	for (const auto& Cmp : SceneComponents)
	{
		Cmp->Begin();
	}

	for (const auto& Cmp : ObjectComponents)
	{
		Cmp->Begin();
	}
}

bool CGameObject::Init()
{
	return true;
}

void CGameObject::Update(const float DeltaTime)
{
	std::erase_if(SceneComponents, [DeltaTime](const auto& Comp)
		{
			if (!Comp || !Comp->GetAlive())
			{
				return true;
			}

			if (Comp->GetEnable())
			{
				Comp->Update(DeltaTime);
			}

			return false;
		});

	std::erase_if(ObjectComponents, [DeltaTime](const auto& Comp)
		{
			if (!Comp || !Comp->GetAlive())
			{
				return true;
			}

			if (Comp->GetEnable())
			{
				Comp->Update(DeltaTime);
			}

			return false;
		});
}

void CGameObject::PostUpdate(const float DeltaTime)
{
	// Scene components
	auto SceneIt = SceneComponents.begin();
	auto SceneEnd = SceneComponents.end();
	while (SceneIt != SceneEnd)
	{
		auto Cmp = *SceneIt;
		if (Cmp.use_count() == 0)
		{
			SceneIt = SceneComponents.erase(SceneIt);
			SceneEnd = SceneComponents.end();
			continue;
		}
		else if (!Cmp->GetAlive())
		{
			SceneIt = SceneComponents.erase(SceneIt);
			SceneEnd = SceneComponents.end();
			continue;
		}
		else if (!Cmp->GetEnable())
		{
			SceneIt = SceneComponents.erase(SceneIt);
			SceneEnd = SceneComponents.end();
			continue;
		}

		Cmp->PostUpdate(DeltaTime);
		++SceneIt;
	}

	// Object components
	auto ObjectIt = ObjectComponents.begin();
	auto ObjectEnd = ObjectComponents.end();
	while (ObjectIt != ObjectEnd)
	{
		auto Cmp = *ObjectIt;
		if (Cmp.use_count() == 0)
		{
			ObjectIt = ObjectComponents.erase(ObjectIt);
			ObjectEnd = ObjectComponents.end();
			continue;
		}
		else if (!Cmp->GetAlive())
		{
			ObjectIt = ObjectComponents.erase(ObjectIt);
			ObjectEnd = ObjectComponents.end();
			continue;
		}
		else if (!Cmp->GetEnable())
		{
			ObjectIt = ObjectComponents.erase(ObjectIt);
			ObjectEnd = ObjectComponents.end();
			continue;
		}

		Cmp->PostUpdate(DeltaTime);
		++ObjectIt;
	}
}

void CGameObject::UpdateTransform()
{
	auto It = SceneComponents.begin();
	while (It != SceneComponents.end())
	{
		auto Cmp = *It;
		if (Cmp.use_count() == 0)
		{
			It = SceneComponents.erase(It);
			continue;
		}

		if (!Cmp->GetAlive())
		{
			It = SceneComponents.erase(It);
			continue;
		}

		if (!Cmp->GetEnable())
		{
			++It;
			continue;
		}

		Cmp->UpdateTransform();
		++It;
	}
}

void CGameObject::Render()
{
	if (auto Root = this->Root.lock())
	{
		Root->Render();
	}
}

void CGameObject::PostRender()
{
	std::erase_if(SceneComponents, [](const std::shared_ptr<CSceneComponent>& Comp)
		{
			if (!Comp || !Comp->GetAlive())
			{
				// TODO: OnDestroy is not implemented in the component (widget), so it is commented out
				// if (Comp) Comp->OnDestroy();
				return true;
			}

			return false;
		});

	for (const auto& Comp : SceneComponents)
	{
		if (Comp->GetEnable())
		{
			Comp->PostRender();
		}
	}

	std::erase_if(ObjectComponents, [](const std::shared_ptr<CObjectComponent>& Comp)
		{
			if (!Comp || !Comp->GetAlive())
			{
				// TODO: OnDestroy is not implemented in the component (widget), so it is commented out
				// if (Comp) Comp->OnDestroy();
				return true;
			}

			return false;
		});

	for (const auto& Comp : ObjectComponents)
	{
		if (Comp->GetEnable())
		{
			Comp->PostRender();
		}
	}
}

CGameObject* CGameObject::Clone()
{
	return new CGameObject(*this);
}

void CGameObject::Destroy()
{
	for (auto& Cmp : SceneComponents)
	{
		Cmp->Destroy();
	}

	for (auto& Cmp : ObjectComponents)
	{
		Cmp->Destroy();
	}

	bAlive = false;
}

float CGameObject::TakeDamage(float Damage)
{
	return 0.f;
}

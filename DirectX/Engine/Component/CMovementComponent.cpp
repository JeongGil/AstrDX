#include "CMovementComponent.h"

#include "CSceneComponent.h"

void CMovementComponent::SetUpdateComponent(const std::weak_ptr<CSceneComponent>& Component)
{
	UpdateComponent = Component;
}

bool CMovementComponent::Init()
{
	if (!CObjectComponent::Init())
	{
		return false;
	}

	return true;
}

void CMovementComponent::Update(const float Delta)
{
	CObjectComponent::Update(Delta);
}

void CMovementComponent::PostUpdate(const float DeltaTime)
{
	CObjectComponent::PostUpdate(DeltaTime);
}

void CMovementComponent::Destroy()
{
	CObjectComponent::Destroy();
}

CMovementComponent* CMovementComponent::Clone() const
{
	return new CMovementComponent(*this);
}

CMovementComponent::CMovementComponent(const CMovementComponent& other) : CObjectComponent(other),
UpdateComponent(other.UpdateComponent)
{
}

CMovementComponent::CMovementComponent(CMovementComponent&& other) noexcept : CObjectComponent(std::move(other)),
UpdateComponent(std::move(other.UpdateComponent))
{
}

CMovementComponent& CMovementComponent::operator=(const CMovementComponent& other)
{
	if (this == &other)
		return *this;
	CObjectComponent::operator =(other);
	UpdateComponent = other.UpdateComponent;
	return *this;
}

CMovementComponent& CMovementComponent::operator=(CMovementComponent&& other) noexcept
{
	if (this == &other)
		return *this;
	CObjectComponent::operator =(std::move(other));
	UpdateComponent = std::move(other.UpdateComponent);
	return *this;
}

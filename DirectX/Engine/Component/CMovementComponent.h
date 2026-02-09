#pragma once
#include "CObjectComponent.h"

#include "../CTimer.h"

class CSceneComponent;

class CMovementComponent :
	public CObjectComponent
{
	friend class CGameObject;

public:
	void SetUpdateComponent(const std::weak_ptr<CSceneComponent>& Component);

protected:
	std::weak_ptr<CSceneComponent> UpdateComponent;

public:
	bool Init() override;
	void Update(const float Delta) override;
	void PostUpdate(const float DeltaTime) override;
	void Destroy() override;

protected:
	CMovementComponent* Clone() const override;

protected:
	CMovementComponent() = default;
	CMovementComponent(const CMovementComponent& other);
	CMovementComponent(CMovementComponent&& other) noexcept;

public:
	~CMovementComponent() override = default;
};


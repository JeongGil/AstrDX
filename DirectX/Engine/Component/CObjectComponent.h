#pragma once
#include "CComponent.h"

class CObjectComponent :
    public CComponent
{
	friend class CGameObject;

public:
	void Begin() override;
	bool Init() override;
	void Update(const float DeltaTime) override;
	void PostUpdate(const float DeltaTime) override;
	void Destroy() override;

protected:
	CComponent* Clone() const override = 0;

	CObjectComponent()
	{
		Type = EType::Object;
	}

	CObjectComponent(const CObjectComponent& other) = default;

	CObjectComponent(CObjectComponent&& other) noexcept = default;

	CObjectComponent& operator=(const CObjectComponent& other) = default;

	CObjectComponent& operator=(CObjectComponent&& other) noexcept = default;

public:
	~CObjectComponent() override = default;
};


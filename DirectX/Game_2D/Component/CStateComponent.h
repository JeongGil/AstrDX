#pragma once
#include "Component/CObjectComponent.h"

class CStateComponent :
    public CObjectComponent
{
	friend class CGameObject;
	friend CObject;

public:
	bool AddHP(int Added);

protected:
	int HP = 10;
	int MaxHP = 10;

protected:
	CComponent* Clone() const override;

protected:
	CStateComponent() = default;
	CStateComponent(const CStateComponent& other) = default;
	CStateComponent(CStateComponent&& other) noexcept = default;
	CStateComponent& operator=(const CStateComponent& other) = default;
	CStateComponent& operator=(CStateComponent&& other) noexcept = default;

public:
	~CStateComponent() override = default;
};


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

	CStateComponent(const CStateComponent& other)
		: CObjectComponent(other),
		  HP(other.HP),
		  MaxHP(other.MaxHP)
	{
	}

	CStateComponent(CStateComponent&& other) noexcept
		: CObjectComponent(std::move(other)),
		  HP(other.HP),
		  MaxHP(other.MaxHP)
	{
	}

	CStateComponent& operator=(const CStateComponent& other)
	{
		if (this == &other)
			return *this;
		CObjectComponent::operator =(other);
		HP = other.HP;
		MaxHP = other.MaxHP;
		return *this;
	}

	CStateComponent& operator=(CStateComponent&& other) noexcept
	{
		if (this == &other)
			return *this;
		CObjectComponent::operator =(std::move(other));
		HP = other.HP;
		MaxHP = other.MaxHP;
		return *this;
	}

public:
	~CStateComponent() override = default;
};


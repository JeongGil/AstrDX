#pragma once
#include "CComponent.h"

class CObjectComponent :
    public CComponent
{
	friend class CGameObject;

public:
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

	CObjectComponent(const CObjectComponent& other)
		: CComponent(other)
	{
	}

	CObjectComponent(CObjectComponent&& other) noexcept
		: CComponent(std::move(other))
	{
	}

	CObjectComponent& operator=(const CObjectComponent& other)
	{
		if (this == &other)
			return *this;
		CComponent::operator =(other);
		return *this;
	}

	CObjectComponent& operator=(CObjectComponent&& other) noexcept
	{
		if (this == &other)
			return *this;
		CComponent::operator =(std::move(other));
		return *this;
	}

public:
	~CObjectComponent() override = default;
};


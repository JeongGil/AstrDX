#pragma once
#include <Component/CObjectComponent.h>

class CHitPointComponent :
    public CObjectComponent
{
	friend class CGameObject;
	friend CObject;

public:


	[[nodiscard]] float GetCurrentHitPoint() const
	{
		return HitPoint;
	}

	[[nodiscard]] float GetMaxHitPoint() const
	{
		return MaxHitPoint;
	}
	void SetMaxHitPoint(float InMaxHitPoint)
	{
		MaxHitPoint = InMaxHitPoint;
	}

	float TakeDamage(float RawDamage, std::weak_ptr<CGameObject> Instigator, std::weak_ptr<CGameObject> Causer);

private:
	float HitPoint = 100;
	float MaxHitPoint = 100;

protected:
	CHitPointComponent() = default;
	CHitPointComponent(const CHitPointComponent& other) = default;
	CHitPointComponent(CHitPointComponent&& other) noexcept = default;
	CHitPointComponent& operator=(const CHitPointComponent& other) = default;
	CHitPointComponent& operator=(CHitPointComponent&& other) noexcept = default;

public:
	~CHitPointComponent() override = default;
};


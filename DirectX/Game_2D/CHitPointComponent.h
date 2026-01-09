#pragma once
#include <Component/CObjectComponent.h>

class CHitPointComponent :
    public CObjectComponent
{
	friend class CGameObject;
	friend CObject;

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
private:
	float HitPoint = 100;
	float MaxHitPoint = 100;
};


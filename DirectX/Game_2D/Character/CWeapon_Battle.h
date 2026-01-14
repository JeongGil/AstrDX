#pragma once
#include <Object/CGameObject.h>

class CColliderBox2D;

class CWeapon_Battle :
    public CGameObject
{
public:
	std::weak_ptr<CGameObject> GetOwner() const
	{
		return Owner;
	}

	void SetOwner(const std::weak_ptr<CGameObject>& Owner)
	{
		this->Owner = Owner;
	}

protected:
	std::weak_ptr<CGameObject> Owner;
	std::weak_ptr<CColliderBox2D> Collider;

	float MoveSpeed = 200.f;
};


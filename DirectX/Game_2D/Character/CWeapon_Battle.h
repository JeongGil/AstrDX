#pragma once
#include <Object/CGameObject.h>

class CInventoryItem_Weapon;
struct FWeaponInfo;
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

	std::weak_ptr<CInventoryItem_Weapon> WeaponData;

	float MoveSpeed = 200.f;
};


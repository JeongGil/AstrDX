#pragma once
#include <Object/CGameObject.h>

#include "../Inventory/CInventoryItem_Weapon.h"

class CMeshComponent;
class CPlayerCharacter;
class CInventoryItem_Weapon;
struct FWeaponInfo;
class CColliderBox2D;

class CWeapon_Battle :
    public CGameObject
{
public:
	bool Init() override;
	void Update(const float DeltaTime) override;

protected:
	CWeapon_Battle* Clone() override;

public:
	std::weak_ptr<CPlayerCharacter> GetOwner() const
	{
		return Owner;
	}

	void SetOwner(const std::weak_ptr<CPlayerCharacter>& Owner)
	{
		this->Owner = Owner;
	}

	TableID GetWeaponInfoID() const
	{
		return WeaponInfoID;
	}

	void SetWeaponInfoID(TableID ID)
	{
		WeaponInfoID = ID;

		InitWeaponInfo(ID);
	}

	void SetWeapon(const std::weak_ptr<CInventoryItem_Weapon>& InvenWeapon)
	{
		Origin = InvenWeapon;

		if (auto Origin = this->Origin.lock())
		{
			InitWeaponInfo(Origin->GetWeaponInfoID());
		}
	}

protected:
	void InitWeaponInfo(TableID ID);

	std::weak_ptr<CPlayerCharacter> Owner;
	std::weak_ptr<CColliderBox2D> Collider;
	std::weak_ptr<CMeshComponent> Mesh;

	TableID WeaponInfoID = TableID(0);
	std::weak_ptr<CInventoryItem_Weapon> Origin;

	float MoveSpeed = 200.f;
};


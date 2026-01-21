#pragma once
#include <Object/CGameObject.h>

#include "../Inventory/CInventoryItem_Weapon.h"
#include <chrono>

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

	void SetPosAnchor(const std::weak_ptr<CSceneComponent>& Anchor)
	{
		PosAnchor = Anchor;
	}

protected:
	void InitWeaponInfo(TableID ID);

private:
	std::weak_ptr<CSceneComponent> GetClosestEnemy();

protected:
	std::weak_ptr<CPlayerCharacter> Owner{};
	std::weak_ptr<CColliderBox2D> Collider{};
	std::weak_ptr<CMeshComponent> Mesh{};

	TableID WeaponInfoID{ -1 };
	std::weak_ptr<CInventoryItem_Weapon> Origin{};

	std::weak_ptr<CSceneComponent> PosAnchor{};

	float MoveSpeed = 200.f;

private:
	std::weak_ptr<CSceneComponent> Target{};

	std::chrono::steady_clock::time_point LastFiredTime{};
	//float Elapsed{};
};


#pragma once
#include <Object/CGameObject.h>

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

protected:
	void InitWeaponInfo(TableID ID);

	std::weak_ptr<CPlayerCharacter> Owner;
	std::weak_ptr<CColliderBox2D> Collider;
	std::weak_ptr<CMeshComponent> Mesh;

	TableID WeaponInfoID = TableID(0);

	float MoveSpeed = 200.f;
};


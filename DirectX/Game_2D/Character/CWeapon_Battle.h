#pragma once
#include <Object/CGameObject.h>

#include "../Inventory/CInventoryItem_Weapon.h"
#include <chrono>

class CEnemy;
class CCollider;
class CColliderSphere2D;
class CMeshComponent;
class CPlayerCharacter;
class CInventoryItem_Weapon;
struct FWeaponInfo;
class CColliderBox2D;

struct FAttackResult
{
	float TotalDamage;
	bool bIsCritical;

	FAttackResult()
		: TotalDamage(0.0f)
		, bIsCritical(false)
	{
	}

	FAttackResult(float InDamage)
		: TotalDamage(InDamage)
		, bIsCritical(false)
	{
	}

	FAttackResult(float InDamage, bool InCritical)
		: TotalDamage(InDamage)
		, bIsCritical(InCritical)
	{
	}
};

class CWeapon_Battle :
	public CGameObject
{
	static constexpr float BULLET_OFFSET = 75.f;

public:
	bool Init() override;
	void Update(float DeltaTime) override;
	void PostUpdate(const float DeltaTime) override;

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

	void OnProjectileCollideOnMonster(const std::weak_ptr<CEnemy>& WeakMonster);

protected:
	void InitWeaponInfo(TableID ID);

private:
	std::weak_ptr<CSceneComponent> GetClosestEnemy();

	static int CalcAttackRange(const FWeaponInfo* WeaponInfo, const std::weak_ptr<CPlayerCharacter>& PlayerCharacter);
	[[nodiscard]] static FAttackResult CalcAttackDamage(const FWeaponInfo* WeaponInfo,
	                                                    const std::weak_ptr<CPlayerCharacter>& PlayerCharacter);

	void OnCollisionBegin(const FVector& HitPoint, CCollider* Other);

	void OnSearchCollisionBlock(const FVector& HitPoint, CCollider* Other);

	float GetTotalMoveTime() const;

private:
	std::weak_ptr<CPlayerCharacter> Owner{};
	std::weak_ptr<CColliderBox2D> Collider{};
	std::weak_ptr<CMeshComponent> Mesh{};

	std::weak_ptr<CColliderSphere2D> SearchCollider{};

	TableID WeaponInfoID{ -1 };
	std::weak_ptr<CInventoryItem_Weapon> Origin{};

	std::weak_ptr<CSceneComponent> PosAnchor{};

	float MoveSpeed = 200.f;

	float ClosestSqrDist = FLT_MAX;
	std::weak_ptr<CGameObject> ClosestEnemy;

	float ElapsedCooldownTime = std::numeric_limits<float>::infinity();

	bool bOnMeleeAttack{};
	float ElapsedMeleeMoveTime = std::numeric_limits<float>::infinity();
	float MovedDistance = std::numeric_limits<float>::infinity();
	FVector TargetDir = FVector::Zero;
};


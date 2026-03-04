#pragma once
#include <Object\CGameObject.h>

class CWeapon_Battle;
class CCharacter;
class CProjectileMovementComponent;
class CColliderBox2D;
class CAnimation2DComponent;
class CMeshComponent;
class CWorld;
class CCollider;

class CProjectile :
	public CGameObject
{
	friend CWorld;
	friend CObject;
protected:
	CProjectile() = default;
	CProjectile(const CProjectile& Other) = default;
	CProjectile(CProjectile&& Other) noexcept = default;

public:
	~CProjectile() override = default;

private:
	std::weak_ptr<CMeshComponent> Mesh;
	std::weak_ptr<CAnimation2DComponent> Animation;
	std::weak_ptr<CColliderBox2D> Collider;
	std::weak_ptr<CProjectileMovementComponent> Movement;

	std::weak_ptr<CCharacter> OwnerCharacter;
	std::weak_ptr<CWeapon_Battle> OwnerWeapon;

	int RemainPenetration = 0;

public:
	bool Init() override;

protected:
	CProjectile* Clone() override;

public:
	void SetEnableMove(bool bEnable) const;

	void SetPenetration(int MaxPenetration)
	{
		RemainPenetration = MaxPenetration;
	}

	void SetCollisionProfile(const std::string& ProfileKey) const;

	void SetMoveDirection(const FVector& Direction) const;

private:
	void OnCollisionBegin(const FVector& HitPoint, CCollider* Other);
	void OnCollisionMapBoundary(const FVector& HitPoint, CCollider* Other);

public:
	[[nodiscard]] std::weak_ptr<CWeapon_Battle> GetOwnerWeapon() const
	{
		return OwnerWeapon;
	}

	void SetOwnerWeapon(const std::weak_ptr<CWeapon_Battle>& OwnerWeapon);

	[[nodiscard]] std::weak_ptr<CCharacter> GetOwnerCharacter() const
	{
		return OwnerCharacter;
	}

	void SetOwnerCharacter(const std::weak_ptr<CCharacter>& Owner)
	{
		this->OwnerCharacter = Owner;
	}
};


#pragma once
#include "CCharacter.h"
#include "../Defines.h"

class CCollider;
class CPlayerCharacter;
class CAnimation2DComponent;

class CEnemy :
	public CCharacter
{
public:
	bool Init() override;
	void Update(float DeltaTime) override;
	float TakeDamage(float Damage, const std::weak_ptr<CGameObject>& Instigator) override;

	[[nodiscard]] TableID GetEnemyInfoID() const
	{
		return EnemyInfoID;
	}

	void SetEnemyInfoID(const TableID& EnemyInfoID);

protected:
	CEnemy* Clone() override;

private:
	void RunBehavior(const std::weak_ptr<CPlayerCharacter>& WeakPlayer);

	void SetChargeCooldownTime(float& OutCooldownTime);

	void OnCollisionBegin(const FVector& HitPoint, CCollider* Collider);

protected:
	static constexpr int CHARGE_MOVE_SPEED = 1000;
	static constexpr int CHARGE_MOVE_DISTANCE = 1000;
	static constexpr int CHARGE_USE_DISTANCE = 800;	

	std::weak_ptr<CMeshComponent> Mesh;
	std::weak_ptr<CAnimation2DComponent> Animation;

	TableID EnemyInfoID{ -1 };
	float MoveSpeed{};

	float MaxHP{};
	float CurrHP{};

	std::weak_ptr<CPlayerCharacter> Player;

	EEnemyBehavior::Type CurrentBehavior{};

	bool bOnCharge{};
	float ChargeCooldownTime = -1.f;
	float ElapsedFromCharge = std::numeric_limits<float>::infinity();
	float ChargeMovedDist{};

	bool bOnKiting{};

	static constexpr int FIRE_COOLDOWN_TIME = 5.f;
	float ElapsedFromFire = std::numeric_limits<float>::infinity();

};


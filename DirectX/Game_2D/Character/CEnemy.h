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

	[[nodiscard]] float GetCurrHP() const
	{
		return CurrHP;
	}

	void SetCurrHP(const float CurrHP);

	void AddCurrHP(float Added)
	{
		SetCurrHP(GetCurrHP() + Added);
	}

protected:
	CEnemy* Clone() override;
	void OnDead() override;
	void UpdateSpawnSequence(float DeltaTime) override;
	void OnSpawnFinished() override;

private:
	void SetChargeCooldownTime(float& OutCooldownTime);

	void OnCollisionBegin(const FVector& HitPoint, CCollider* Collider);
	void OnCollisionEnd(CCollider* Collider);
	void CollidingWithPC(const std::weak_ptr<CPlayerCharacter>& WeakPC);

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
	bool bIsCollidingToPC{};

	EEnemyBehavior::Type CurrentBehavior{};

	bool bOnCharge{};
	float ChargeCooldownTime = -1.f;
	float ElapsedFromCharge = std::numeric_limits<float>::infinity();
	float ChargeMovedDist{};

	bool bOnKiting{};

	static constexpr int FIRE_COOLDOWN_TIME = 5.f;
	float ElapsedFromFire = std::numeric_limits<float>::infinity();

	static constexpr float SPAWN_FLICKER_INTERVAL = 0.4f;
	float ElapsedBirthFlickeringTime{};
	std::weak_ptr<CMeshComponent> BirthMarker;
};


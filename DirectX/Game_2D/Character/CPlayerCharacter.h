#pragma once
#include "CCharacter.h"
#include "../Defines.h"

#include "../Table/CharacterVisualInfo.h"

class CCollider;
class CColliderSphere2D;
class CCameraComponent;
class CInventoryItem_Weapon;
class CWeapon_Battle;
class CObjectMovementComponent;
class CAnimation2DComponent;
struct FCharacterVisualInfo;

class CPlayerCharacter :
	public CCharacter
{
	friend class CWorld;
	friend CObject;

	constexpr static float INVINCIBLE_DURATION = 0.5f;
	constexpr static float INVINCIBLE_FLICKER_INTERVAL = 0.2f;

public:
	bool Init() override;
	void Update(float DeltaTime) override;
	void PostUpdate(const float DeltaTime) override;
	float TakeDamage(float Damage, const std::weak_ptr<CGameObject>& Instigator) override;

	void SetCharacterVisual(TableID VisualInfoID);

	void AddWeapon(const std::weak_ptr<CInventoryItem_Weapon>& Weapon);

	float GetStat(EStat::Type StatType);

	static int GetToolTipDmgReductionPercent(int Armor);

protected:
	CPlayerCharacter* Clone() override;
	void OnDead() override;
	void UpdateSpawnSequence(float DeltaTime) override;

private:
	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();

	void CreateDeco(const std::string& DecoPath);

	void SetAnchorPosition(size_t WeaponCount);

	static float GetArmoredDmgRatio(int Armor);

	void OnPickupColliderBeginOverlap(const FVector& HitPoint, CCollider* Other);

	void SetBaseStatus();

	void SetBodyColor(const FVector4& Color);
	void OnHitEffectAnimFinished();

protected:
	TableID CharacterVisualInfoID = TableID(-1);

	std::weak_ptr<CCameraComponent> Camera;

	std::weak_ptr<CMeshComponent> Potato;
	std::weak_ptr<CMeshComponent> Leg;
	std::vector<std::weak_ptr<CMeshComponent>> Decos;

	std::weak_ptr<CAnimation2DComponent> PotatoAnim;
	std::weak_ptr<CAnimation2DComponent> LegAnim;
	std::vector<std::weak_ptr<CAnimation2DComponent>> DecoAnims;

	std::vector<std::weak_ptr<CWeapon_Battle>> Weapons;
	std::array<std::weak_ptr<CSceneComponent>, INVENTORY_MAX_WEAPON> WeaponAnchors;

	std::weak_ptr<CObjectMovementComponent> MovementComponent;

	std::weak_ptr<CColliderSphere2D> PickupCollider;

	std::weak_ptr<CMeshComponent> HitEffectMesh;
	std::weak_ptr<CAnimation2DComponent> HitEffectAnim;

	int RemainAbsorbAttackStack = 0;

	std::unordered_map<EStat::Type, float> BaseStats;
	std::unordered_map<EStat::Type, float> UpgradeStats;

	float CurrHP{};

	bool bLastSymmetry = false;

	float ElapsedFromDamaged = INVINCIBLE_DURATION;

private:
	inline static std::unordered_map<size_t, std::vector<FVector2>> AnchorPositions =
	{
		{ 1, { FVector2(0, -25) } },
		{ 2, { FVector2(30, -25), FVector2(-30, -25) } },
		{ 3, { FVector2(30, -25), FVector2(-30, -25), FVector2(0, 50) } },
		{ 4, { FVector2(30, -25), FVector2(-30, -25), FVector2(-30, 25), FVector2(30, 25) } },
		{ 5, { FVector2(30, -25), FVector2(-30, -25), FVector2(-30, 25), FVector2(30, 25), FVector2(0, 50) } },
		{ 6, { FVector2(30, -30), FVector2(-30, -30), FVector2(-50, 0), FVector2(-30, 30), FVector2(30, 30), FVector2(50, 0) } }
	};

protected:
	CPlayerCharacter() = default;
	CPlayerCharacter(const CPlayerCharacter& other) = default;
	CPlayerCharacter(CPlayerCharacter&& other) noexcept = default;
	CPlayerCharacter& operator=(const CPlayerCharacter& other) = default;
	CPlayerCharacter& operator=(CPlayerCharacter&& other) noexcept = default;

public:
	~CPlayerCharacter() override = default;

	[[nodiscard]] float GetCurrHP() const
	{
		return CurrHP;
	}

	void SetCurrHP(float InCurrHP)
	{
		InCurrHP = max(0.f, InCurrHP);
		InCurrHP = std::clamp(InCurrHP, 0.f, GetStat(EStat::MaxHP));
		CurrHP = InCurrHP;

		if (CurrHP == 0.f)
		{
			OnDead();
		}
	}
};


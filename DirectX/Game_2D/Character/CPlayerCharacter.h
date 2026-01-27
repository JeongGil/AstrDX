#pragma once
#include "CCharacter.h"
#include "../Defines.h"

#include "../Table/CharacterVisualInfo.h"

class CCameraComponent;
class CInventoryItem_Weapon;
class CWeapon_Battle;
class CObjectMovementComponent;
struct FCharacterVisualInfo;

class CPlayerCharacter :
	public CCharacter
{
	friend class CWorld;
	friend CObject;

public:
	bool Init() override;
	void Update(float DeltaTime) override;
	void PostUpdate(const float DeltaTime) override;

	void SetCharacterVisual(TableID VisualInfoID);

	void AddWeapon(const std::weak_ptr<CInventoryItem_Weapon>& Weapon);

protected:
	CPlayerCharacter* Clone() override;

private:
	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();

	void CreateDeco(const std::string& DecoPath);

	void SetAnchorPosition(size_t WeaponCount);

protected:
	TableID CharacterVisualInfoID = TableID(-1);

	std::weak_ptr<CCameraComponent> Camera;

	std::weak_ptr<CMeshComponent> Potato;
	std::weak_ptr<CMeshComponent> Leg;
	std::vector<std::weak_ptr<CMeshComponent>> Decos;

	std::vector<std::weak_ptr<CWeapon_Battle>> Weapons;
	std::array<std::shared_ptr<CSceneComponent>, INVENTORY_MAX_WEAPON> WeaponAnchors;

	std::weak_ptr<CObjectMovementComponent> MovementComponent;

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
};


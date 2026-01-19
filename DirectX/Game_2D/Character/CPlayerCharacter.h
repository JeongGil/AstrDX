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
	void Update(const float DeltaTime) override;
	void PostUpdate(const float DeltaTime) override;
	
	void SetCharacterVisual(TableID VisualInfoID);

	void SetWeapon(const std::weak_ptr<CInventoryItem_Weapon>& Weapon, size_t SlotIdx);

protected:
	CPlayerCharacter* Clone() override;

private:
	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();

	void CreateDeco(const std::string& DecoPath);

protected:
	TableID CharacterVisualInfoID = TableID(-1);

	std::weak_ptr<CCameraComponent> Camera;

	std::weak_ptr<CMeshComponent> Potato;
	std::weak_ptr<CMeshComponent> Leg;
	std::vector<std::weak_ptr<CMeshComponent>> Decos;

	std::vector<std::weak_ptr<CWeapon_Battle>> Weapons;

	std::weak_ptr<CObjectMovementComponent> MovementComponent;

protected:
	CPlayerCharacter() = default;
	CPlayerCharacter(const CPlayerCharacter& other) = default;
	CPlayerCharacter(CPlayerCharacter&& other) noexcept = default;
	CPlayerCharacter& operator=(const CPlayerCharacter& other) = default;
	CPlayerCharacter& operator=(CPlayerCharacter&& other) noexcept = default;

public:
	~CPlayerCharacter() override = default;
};


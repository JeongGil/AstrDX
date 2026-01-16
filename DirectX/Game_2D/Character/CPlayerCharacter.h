#pragma once
#include "CCharacter.h"
#include "../Defines.h"

#include "../Table/CharacterVisualInfo.h"

class CWeapon_Inventory;
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

	void SetCharacterVisual(const FCharacterVisualInfo& VisualInfo);

	void SetWeapon(const std::weak_ptr<CWeapon_Inventory>& Weapon, size_t SlotIdx);

protected:
	CPlayerCharacter* Clone() override;

private:
	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();

protected:
	FCharacterVisualInfo CharacterVisualInfo;

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


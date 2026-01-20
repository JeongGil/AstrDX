#pragma once

#include <EngineInfo.h>

#include "../Defines.h"

struct FShopGoods;
class CInventoryItem_Item;
class CInventoryItem_Weapon;

class CInventory
{
public:
	bool Init();

	std::weak_ptr<CInventoryItem_Weapon> GetWeapon(size_t SlotIdx);
	size_t GetWeaponCount() const
	{
		return Weapons.size();
	}

	void AddWeapon(const FShopGoods& WeaponGoods);
	void AddWeapon(TableID WeaponID);
	void RemoveWeapon(size_t SlotIdx);
	void RemoveWeapon(const std::weak_ptr<CInventoryItem_Weapon>& Weapon);

	void Clear()
	{
		Weapons.clear();
		Items.clear();
	}

private:
	std::vector<std::shared_ptr<CInventoryItem_Weapon>> Weapons;
	std::vector<std::shared_ptr<CInventoryItem_Item>> Items;

public:
	static CInventory& GetInst()
	{
		static CInventory Inst;
		return Inst;
	}

private:
	CInventory() = default;
	~CInventory() = default;
};


#include "CInventory.h"

#include "CInventoryItem_Weapon.h"
#include "CShop.h"
#include "../Table/WeaponInfo.h"
#include "../Table/WeaponTable.h"

bool CInventory::Init()
{
	Weapons.reserve(INVENTORY_MAX_WEAPON);

	return true;
}

std::weak_ptr<CInventoryItem_Weapon> CInventory::GetWeapon(size_t SlotIdx)
{
	assert(SlotIdx < INVENTORY_MAX_WEAPON);

	if (SlotIdx >= Weapons.size())
	{
		return std::weak_ptr<CInventoryItem_Weapon>();
	}

	return Weapons[SlotIdx];
}

void CInventory::AddWeapon(const FShopGoods& WeaponGoods)
{
	assert(WeaponGoods.bIsWeapon);

	if (Weapons.size() >= INVENTORY_MAX_WEAPON)
	{
		return;
	}
}

void CInventory::AddWeapon(TableID WeaponID)
{
	if (Weapons.size() >= INVENTORY_MAX_WEAPON)
	{
		return;
	}

	FWeaponInfo* Info;
	if (!WeaponTable::GetInst().TryGet(WeaponID, Info))
	{
		return;
	}

	auto Added = Weapons.emplace_back(std::make_shared<CInventoryItem_Weapon>());
	Added->SetWeaponInfoID(WeaponID);
}

void CInventory::RemoveWeapon(size_t SlotIdx)
{
	assert(SlotIdx < INVENTORY_MAX_WEAPON);

	if (SlotIdx < Weapons.size())
	{
		Weapons.erase(Weapons.begin() + SlotIdx);
	}
}

void CInventory::RemoveWeapon(const std::weak_ptr<CInventoryItem_Weapon>& Weapon)
{
	auto InvenWeapon = Weapon.lock();
	if (!InvenWeapon)
	{
		return;
	}

	auto It = Weapons.begin();
	while (It != Weapons.end())
	{
		if (It->get() == InvenWeapon.get())
		{
			Weapons.erase(It);
			return;
		}
	}
}

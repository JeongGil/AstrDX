#include "CInventoryData.h"

#include "CInventoryItem_Weapon.h"
#include "CShop.h"
#include "../Table/WeaponInfo.h"
#include "../Table/WeaponTable.h"

bool CInventoryData::Init()
{
	Weapons.reserve(INVENTORY_MAX_WEAPON);

	return true;
}

std::weak_ptr<CInventoryItem_Weapon> CInventoryData::GetWeapon(size_t SlotIdx)
{
	assert(SlotIdx < INVENTORY_MAX_WEAPON);

	if (SlotIdx >= Weapons.size())
	{
		return std::weak_ptr<CInventoryItem_Weapon>();
	}

	return Weapons[SlotIdx];
}

void CInventoryData::AddWeapon(const FShopGoods& WeaponGoods)
{
	assert(WeaponGoods.bIsWeapon);

	if (Weapons.size() >= INVENTORY_MAX_WEAPON)
	{
		return;
	}

	// TODO: 작업 필요.
	assert(false);

	RefreshWeaponTypeCounts();
}

void CInventoryData::AddWeapon(TableID WeaponID)
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

	RefreshWeaponTypeCounts();
}

void CInventoryData::RemoveWeapon(size_t SlotIdx)
{
	assert(SlotIdx < INVENTORY_MAX_WEAPON);

	if (SlotIdx < Weapons.size())
	{
		Weapons.erase(Weapons.begin() + SlotIdx);

		RefreshWeaponTypeCounts();
	}
}

void CInventoryData::RemoveWeapon(const std::weak_ptr<CInventoryItem_Weapon>& Weapon)
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

			RefreshWeaponTypeCounts();

			return;
		}
	}
}

void CInventoryData::RefreshWeaponTypeCounts()
{
	WeaponTypeCounts.clear();

	for (const auto& Weapon : Weapons)
	{
		FWeaponInfo* Info;
		if (WeaponTable::GetInst().TryGet(Weapon->GetWeaponInfoID(), Info))
		{
			for (int i = 0; i <= 16; i++)
			{
				auto Type = static_cast<EWeaponType::Type>(1 << i);
				if (Info->WeaponType & Type)
				{
					++WeaponTypeCounts[Type];
				}
			}
		}
	}
}

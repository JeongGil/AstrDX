#include "CCharacterData.h"

#include "CInventoryItem_Item.h"
#include "CInventoryItem_Weapon.h"
#include "CShop.h"
#include "../Table/CharacterBaseTable.h"
#include "../Table/ItemTable.h"
#include "../Table/WeaponInfo.h"
#include "../Table/WeaponSetBonusTable.h"
#include "../Table/WeaponTable.h"

bool CCharacterData::Init()
{
	Clear();

	Weapons.reserve(INVENTORY_MAX_WEAPON);

	SetMaterialCount(1000);
	Exp = 0;

	SetBaseStatus();

	// 임시
	AddWeapon(TableID(1));
	AddWeapon(TableID(2));
	AddWeapon(TableID(5));

	return true;
}

float CCharacterData::GetStat(EStat::Type StatType) const
{
	float Value{};

	if (auto It = BaseStats.find(StatType); It != BaseStats.end())
	{
		Value += It->second;
	}

	if (auto It = UpgradeStats.find(StatType); It != UpgradeStats.end())
	{
		Value += It->second;
	}

	for (const auto& [ItemID, Item] : Items)
	{
		if (!Item)
		{
			continue;
		}

		FItemInfo* ItemInfo = nullptr;
		if (!ItemTable::GetInst().TryGet(ItemID, ItemInfo))
		{
			continue;
		}

		auto EffectIt = ItemInfo->Effects.find(StatType);
		if (EffectIt != ItemInfo->Effects.end())
		{
			Value += static_cast<float>(EffectIt->second * Item->GetItemCount());
		}
	}

	for (const auto& [WeaponType, TypeCount] : WeaponTypeCounts)
	{
		FWeaponSetBonusInfo* Info;

		TableID ID(static_cast<int>(WeaponType));
		if (WeaponSetBonusTable::GetInst().TryGet(ID, Info))
		{
			auto Bonus = Info->Bonus1[TypeCount - 1];
			if (Bonus.StatType == StatType)
			{
				Value += Bonus.StatValue;
			}

			Bonus = Info->Bonus2[TypeCount - 1];
			if (Bonus.StatType == StatType)
			{
				Value += Bonus.StatValue;
			}
		}
	}

	return Value;
}

void CCharacterData::SetBaseStatus()
{
	BaseStats.clear();

	BaseStats.emplace(EStat::Level, 0);

	if (auto CharBaseInfo = CharacterBaseTable::GetInst().Get())
	{
		BaseStats.emplace(EStat::MaxHP, CharBaseInfo->BaseHP);
	}
}

std::weak_ptr<CInventoryItem_Weapon> CCharacterData::GetWeapon(size_t SlotIdx)
{
	assert(SlotIdx < INVENTORY_MAX_WEAPON);

	if (SlotIdx >= Weapons.size())
	{
		return std::weak_ptr<CInventoryItem_Weapon>();
	}

	return Weapons[SlotIdx];
}

void CCharacterData::AddWeapon(const FShopGoods& WeaponGoods)
{
	assert(WeaponGoods.bIsWeapon);

	if (Weapons.size() >= INVENTORY_MAX_WEAPON)
	{
		return;
	}

	AddWeapon(WeaponGoods.GoodsID);
}

void CCharacterData::AddWeapon(TableID WeaponID)
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

void CCharacterData::RemoveWeapon(size_t SlotIdx)
{
	assert(SlotIdx < INVENTORY_MAX_WEAPON);

	if (SlotIdx < Weapons.size())
	{
		Weapons.erase(Weapons.begin() + SlotIdx);

		RefreshWeaponTypeCounts();
	}
}

void CCharacterData::RemoveWeapon(const std::weak_ptr<CInventoryItem_Weapon>& Weapon)
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

int CCharacterData::GetItemCount(TableID ItemID) const
{
	auto It = Items.find(ItemID);
	if (It == Items.end())
	{
		return 0;
	}

	return It->second->GetItemCount();
}

void CCharacterData::AddItem(const FShopGoods& ItemGoods)
{
	assert(!ItemGoods.bIsWeapon);

	auto It = Items.find(ItemGoods.GoodsID);
	if (It != Items.end())
	{
		It->second->SetItemCount(It->second->GetItemCount() + 1);
	}
	else
	{
		auto NewItem = std::make_shared<CInventoryItem_Item>();
		NewItem->SetItemInfoID(ItemGoods.GoodsID);
		NewItem->SetItemCount(1);
		Items.emplace(ItemGoods.GoodsID, std::move(NewItem));
	}
}

void CCharacterData::RefreshWeaponTypeCounts()
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

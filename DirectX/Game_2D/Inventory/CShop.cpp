#include "CShop.h"

#include "CCharacterData.h"
#include "../Table/ItemInfo.h"
#include "../Table/ItemTable.h"
#include "../Table/WeaponTable.h"
#include <random>
#include <CEngine.h>

int FShopGoods::GetPrice() const
{
	int Price = -1;
	int Base = 0;
	if (bIsWeapon)
	{
		FWeaponInfo* Info;
		if (!WeaponTable::GetInst().TryGet(GoodsID, Info))
		{
			return Price;
		}

		Base = Info->BasePrice;

	}
	else
	{
		FItemInfo* Info;
		if (!ItemTable::GetInst().TryGet(GoodsID, Info))
		{
			return Price;
		}

		Base = Info->BasePrice;
	}

	auto Wave = CCharacterData::GetInst().GetStageLevel();
	Price = static_cast<int>(std::floor(Base + Wave + Base * Wave * 0.1f));

	const static TableID CouponID{ -1 };
	int CouponCount = CCharacterData::GetInst().GetItemCount(CouponID);
	Price -= 1 - 0.05 * CouponCount;
	Price = max(1, Price);

	return Price;
}

void CShop::GenerateGoods()
{
	++RerollCount;
	RerollPrice = RerollCount * 5;

	for (size_t i = 0; i < SHOP_MAX_GOODS; i++)
	{
		CurrentGoods[i] = {};
		GenerateRandomGoods(CurrentGoods[i].GoodsID, CurrentGoods[i].bIsWeapon);
	}
}

bool CShop::GenerateRandomGoods(TableID& OutID, bool& OutIsWeapon)
{
	const auto& WeaponItems = WeaponTable::GetInst().GetItems();
	const auto& ItemItems = ItemTable::GetInst().GetItems();

	const size_t TotalCount = WeaponItems.size() + ItemItems.size();
	if (TotalCount == 0)
	{
		return false;
	}

	std::uniform_int_distribution<size_t> Dist(0, TotalCount - 1);
	const size_t PickIdx = Dist(CEngine::GetInst()->GetMT());

	if (PickIdx < WeaponItems.size())
	{
		auto It = WeaponItems.begin();
		std::advance(It, PickIdx);
		OutID = It->first;
		OutIsWeapon = true;
	}
	else
	{
		auto It = ItemItems.begin();
		std::advance(It, PickIdx - WeaponItems.size());
		OutID = It->first;
		OutIsWeapon = false;
	}

	return true;
}

bool CShop::Reroll()
{
	if (CCharacterData::GetInst().GetMaterialCount() < RerollPrice)
	{
		return false;
	}

	CCharacterData::GetInst().AddMaterialCount(-RerollPrice);
	GenerateGoods();
	return true;
}

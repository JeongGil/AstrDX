#include "CShop.h"

#include "CCharacterData.h"
#include "../Table/ItemInfo.h"
#include "../Table/ItemTable.h"
#include "../Table/WeaponTable.h"

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

//CCharacterData::GetInst().

	return Price;
}

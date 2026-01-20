#pragma once

#include <EngineInfo.h>

struct FShopGoods
{
	bool bIsWeapon = false;
	TableID GoodsID = TableID(-1);
};

class CShop
{
	constexpr static size_t SHOP_MAX_GOODS = 3;

private:
	std::array<FShopGoods, SHOP_MAX_GOODS> CurrentGoods = {};
};


#pragma once

#include <EngineInfo.h>

struct FShopGoods
{
	bool bSoldOut = false;

	bool bIsWeapon = false;
	TableID GoodsID = TableID(-1);
	[[nodiscard]] int GetPrice() const;
};

class CShop
{
	constexpr static size_t SHOP_MAX_GOODS = 4;

private:
	std::array<FShopGoods, SHOP_MAX_GOODS> CurrentGoods = {};

	int RerollCount = -1;
	int RerollPrice = 0;

public:
	[[nodiscard]] const FShopGoods& GetGoodsInfo(size_t SlotIdx) const
	{
		assert(SlotIdx < SHOP_MAX_GOODS);
		return CurrentGoods[SlotIdx];
	}

	[[nodiscard]] int GetRerollPrice() const
	{
		return RerollPrice;
	}

	void GenerateGoods();
	bool Reroll();

private:
	bool GenerateRandomGoods(TableID& OutID, bool& OutIsWeapon);

private:
	CShop() = default;
	~CShop() = default;

public:
	static CShop& GetInst()
	{
		static CShop Inst;
		return Inst;
	}
};


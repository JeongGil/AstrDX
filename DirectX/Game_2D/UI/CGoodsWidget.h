#pragma once
#include <UI/CWidgetContainer.h>

#include "../Inventory/CShop.h"

class CGoodsSlot;
class CWorldUIManager;

class CGoodsWidget :
    public CWidgetContainer
{
	friend CWorldUIManager;
	friend CWidgetContainer;

protected:
	CGoodsWidget() = default;

public:
	~CGoodsWidget() override = default;

private:
	std::array<std::weak_ptr<CGoodsSlot>, 4> Slots;

public:
	bool Init() override;

	void SetGoods(const FShopGoods& GoodsInfo, size_t SlotIdx);
};


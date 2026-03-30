#pragma once
#include <UI/CWidgetContainer.h>

#include "../Inventory/CShop.h"

class CImage;
class CButton;
class CTextBlock;
class CItemSlot;
class CWorldUIManager;

class CGoodsSlot :
	public CWidgetContainer
{
	friend CWorldUIManager;
	friend CWidgetContainer;

protected:
	CGoodsSlot() = default;

public:
	~CGoodsSlot() override = default;

private:
	int SlotIdx = -1;

	std::weak_ptr<CImage> Background;

	std::weak_ptr<CItemSlot> Slot;
	std::weak_ptr<CTextBlock> Name;

	std::weak_ptr<CButton> BuyButton;
	std::weak_ptr<CImage> PriceIcon;
	std::weak_ptr<CTextBlock> PriceText;

	FShopGoods GoodsInfo;

public:
	bool Init() override;

	void SetItem(const FShopGoods& InGoodsInfo);

	[[nodiscard]] int GetSlotIdx() const
	{
		return SlotIdx;
	}

	void SetSlotIdx(const int SlotIdx)
	{
		this->SlotIdx = SlotIdx;
	}

private:
	void SetItemInfo();
	void SetWeaponInfo();

	void OnClickBuy();
};


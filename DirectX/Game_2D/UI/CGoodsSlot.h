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
	struct FItemEffectLineWidgets
	{
		std::weak_ptr<CImage> Icon;
		std::weak_ptr<CTextBlock> Value;
		std::weak_ptr<CTextBlock> Name;
	};

	struct FWeaponInfoLineWidgets
	{
		std::weak_ptr<CTextBlock> Label;
		std::weak_ptr<CTextBlock> Value;
	};

private:
	int SlotIdx = -1;

	std::weak_ptr<CImage> Background;

	std::weak_ptr<CItemSlot> Slot;
	std::weak_ptr<CTextBlock> Name;

	std::weak_ptr<CButton> BuyButton;
	std::weak_ptr<CImage> PriceIcon;
	std::weak_ptr<CTextBlock> PriceText;

	std::vector<FItemEffectLineWidgets> ItemEffectLines;
	std::vector<FWeaponInfoLineWidgets> WeaponInfoLines;

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
	void HideItemEffectLines();
	void HideWeaponInfoLines();

	void OnClickBuy();
};


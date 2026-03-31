#include "CItemInvenWidget.h"
#include "CItemInvenWidget.h"

#include <CDevice.h>
#include <UI/CTextBlock.h>

#include "CItemSlot.h"
#include "../Inventory/CCharacterData.h"
#include "../Inventory/CInventoryItem_Item.h"

bool CItemInvenWidget::Init()
{
	if (!CWidgetContainer::Init())
	{
		return false;
	}

	const auto Ratio = CDevice::GetInst()->GetRatioFHD();
	SlotSize = 96.f * Ratio;
	Pad = 4.f * Ratio;

	Refresh();

	Title = CreateWidget<CTextBlock>("Title", 2);
	if (auto Text = Title.lock())
	{
		const FVector TitleSize = FVector(400.f, 45.f, 0.f) * Ratio;
		Text->SetPos(0.f, -TitleSize.y);
		Text->SetSize(TitleSize);
		Text->SetText(TEXT("아이템"));
		Text->SetTextColor(FColor::White);
		Text->SetAlignH(ETextAlignH::Left);
		Text->SetAlignV(ETextAlignV::Top);
	}

	return true;
}

void CItemInvenWidget::Refresh()
{
	const auto& Items = CCharacterData::GetInst().GetItems();

	const int NeedCount = static_cast<int>(Items.size());

	while (static_cast<int>(Slots.size()) < NeedCount)
	{
		int Idx = static_cast<int>(Slots.size());
		auto Slot = CreateWidget<CItemSlot>("ItemSlot", 1);
		if (auto S = Slot.lock())
		{
			const int Col = Idx % COLS;
			const int Row = Idx / COLS;
			S->SetPos((SlotSize + Pad) * Col, (SlotSize + Pad) * Row);
			S->SetSize(SlotSize, SlotSize);
			S->SetFontSize(SlotSize * 0.22f);
		}
		Slots.push_back(Slot);
	}

	int Idx = 0;
	for (const auto& [ID, ItemPtr] : Items)
	{
		if (auto Slot = Slots[Idx].lock())
		{
			Slot->SetItem(ID, false);
			Slot->SetItemCount(ItemPtr->GetItemCount());
			Slot->SetEnable(true);
		}
		++Idx;
	}
}


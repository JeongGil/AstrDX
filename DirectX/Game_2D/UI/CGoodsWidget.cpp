#include "CGoodsWidget.h"

#include <CDevice.h>

#include "CGoodsSlot.h"

bool CGoodsWidget::Init()
{
	if (!CWidgetContainer::Init())
	{
		return false;
	}

	const auto Ratio = CDevice::GetInst()->GetRatioFHD();

	auto Pos = FVector(26, 142, 0) * Ratio;
	SetPos(Pos);

	const auto Pad = 10 * Ratio;
	FVector SlotPos{};
	for (int i = 0; i < Slots.size(); i++)
	{
		Slots[i] = CreateWidget<CGoodsSlot>("Slot", 1);
		if (auto Slot = Slots[i].lock())
		{
			Slot->SetPos(SlotPos);
			SlotPos.x += Slot->GetSize().x + Pad;

			Slot->SetSlotIdx(i);
		}
	}

	return true;
}

void CGoodsWidget::SetGoods(TableID ID, bool bIsWeapon, size_t SlotIdx)
{
	assert(SlotIdx < Slots.size());
	if (auto Slot = Slots[SlotIdx].lock())
	{
		Slot->SetItem(ID, bIsWeapon);
	}
}

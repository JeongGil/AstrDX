#include "CWeaponInvenWidget.h"

#include <CDevice.h>

#include "CItemSlot.h"
#include "../Inventory/CCharacterData.h"

bool CWeaponInvenWidget::Init()
{
	if (!CWidgetContainer::Init())
	{
		return false;
	}

	const auto Ratio = CDevice::GetInst()->GetRatioFHD();

	const auto SlotSize = 96 * Ratio;
	for (int i = 0; i < Slots.size(); i++)
	{
		Slots[i] = CreateWidget<CItemSlot>("Slot", 1);
		if (auto Slot = Slots[i].lock())
		{
			Slot->SetSize(SlotSize, SlotSize);

			bool bHasWeapon = i < CCharacterData::GetInst().GetWeaponCount();
			Slot->SetEnable(bHasWeapon);
		}
	}

	return true;
}

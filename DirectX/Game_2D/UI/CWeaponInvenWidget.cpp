#include "CWeaponInvenWidget.h"

#include <CDevice.h>

#include "CItemSlot.h"
#include "../Inventory/CCharacterData.h"
#include "../Inventory/CInventoryItem_Weapon.h"

bool CWeaponInvenWidget::Init()
{
	if (!CWidgetContainer::Init())
	{
		return false;
	}

	const auto Ratio = CDevice::GetInst()->GetRatioFHD();

	constexpr int Cols = 3;
	const float SlotSize = 96.f * Ratio;
	const float Pad = 4.f * Ratio;

	for (int i = 0; i < static_cast<int>(Slots.size()); i++)
	{
		Slots[i] = CreateWidget<CItemSlot>("Slot", 1);
		if (auto Slot = Slots[i].lock())
		{
			const int Col = i % Cols;
			const int Row = i / Cols;
			Slot->SetPos((SlotSize + Pad) * Col, (SlotSize + Pad) * Row);
			Slot->SetSize(SlotSize, SlotSize);
			Slot->SetFontSize(SlotSize * 0.22f);
		}
	}

	Refresh();

	return true;
}

void CWeaponInvenWidget::Refresh()
{
	auto& CharData = CCharacterData::GetInst();
	const size_t WeaponCount = CharData.GetWeaponCount();

	for (int i = 0; i < static_cast<int>(Slots.size()); i++)
	{
		auto Slot = Slots[i].lock();
		if (!Slot)
		{
			continue;
		}

		if (i < static_cast<int>(WeaponCount))
		{
			auto Weapon = CharData.GetWeapon(static_cast<size_t>(i));
			if (auto W = Weapon.lock())
			{
				Slot->SetItem(W->GetWeaponInfoID(), true);
			}
			Slot->SetItemCount(1);
			Slot->SetEnable(true);
		}
		else
		{
			Slot->SetEnable(false);
		}
	}
}

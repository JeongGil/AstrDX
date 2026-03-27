#include "CGoodsSlot.h"

#include <CDevice.h>
#include <UI/CImage.h>
#include <UI/CTextBlock.h>

#include "CItemSlot.h"
#include "../Table/ItemInfo.h"
#include "../Table/ItemTable.h"

bool CGoodsSlot::Init()
{
	if (!CWidgetContainer::Init())
	{
		return false;
	}

	const auto Ratio = CDevice::GetInst()->GetRatioFHD();
	auto Size = FVector(351, 483, 0) * Ratio;
	
	SetSize(Size);

	Background = CreateWidget<CImage>("Background", 1);
	if (auto Image = Background.lock())
	{
		Image->SetSize(GetSize());
		Image->SetTint(FColor::HalfTransparent);
	}

	Slot = CreateWidget<CItemSlot>("Slot", 2);
	if (auto Slot = this->Slot.lock())
	{
		auto Pad = 14 * Ratio;
		Slot->SetPos(Pad, Pad);

		auto SlotWidth = 100 * Ratio;
		Slot->SetSize(SlotWidth, SlotWidth);
	}

	Name = CreateWidget<CTextBlock>("Name", 2);
	if (auto Text = Name.lock())
	{
		if (auto Slot = this->Slot.lock())
		{
			auto Pos = Slot->GetPos();
			Pos.x += Slot->GetSize().x + 14 * Ratio;

			Text->SetPos(Pos);
		}

		Size = FVector(30, 250, 0) * Ratio;
		Text->SetSize(Size);

		Text->SetAlignH(ETextAlignH::Left);
		Text->SetAlignV(ETextAlignV::Top);

		auto FontSize = 26 * Ratio;
		Text->SetFontSize(FontSize);
	}
	return true;
}

void CGoodsSlot::SetItem(TableID ID, bool bWeapon)
{
	if (ItemID == ID && bIsWeapon == bWeapon)
	{
		return;
	}

	ItemID = ID;
	bIsWeapon = bWeapon;

	if (auto Slot = this->Slot.lock())
	{
		Slot->SetItem(ID, bWeapon);
	}

	if (bWeapon)
	{
		SetWeaponInfo();
	}
	else
	{
		SetItemInfo();
	}
}

void CGoodsSlot::SetItemInfo()
{
	FItemInfo* Info;
	if (!ItemTable::GetInst().TryGet(ItemID, Info))
	{
		return;
	}

	
}

void CGoodsSlot::SetWeaponInfo()
{
}

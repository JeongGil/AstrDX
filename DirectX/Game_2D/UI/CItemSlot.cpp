#include "CItemSlot.h"

#include <UI/CImage.h>
#include <UI/CTextBlock.h>
#include "../Strings.h"
#include "../Utility.h"
#include "../Table/ItemInfo.h"
#include "../Table/ItemTable.h"
#include "../Table/WeaponTable.h"

bool CItemSlot::Init()
{
	if (!CWidgetContainer::Init())
	{
		return false;
	}

	Background = CreateWidget<CImage>("Background", GetZOrder());
	if (auto Image = Background.lock())
	{
		Image->SetTint(BASE_BACKGROUND_COLOR);
	}

	SlotItem = CreateWidget<CImage>("SlotItem", GetZOrder() + 1);
	if (auto Image = SlotItem.lock())
	{
		Image->SetEnable(false);
	}

	ItemCount = CreateWidget<CTextBlock>("ItemCount", GetZOrder() + 2);

	SetSize(GetSize());

	return true;
}

void CItemSlot::SetZOrder(const int ZOrder)
{
	CWidgetContainer::SetZOrder(ZOrder);

	if (auto Image = Background.lock())
	{
		Image->SetZOrder(ZOrder);
	}

	if (auto Image = SlotItem.lock())
	{
		Image->SetZOrder(ZOrder + 1);
	}

	if (auto Text = ItemCount.lock())
	{
		Text->SetZOrder(ZOrder + 1);
	}
}

void CItemSlot::SetSize(const FVector& Size)
{
	CWidgetContainer::SetSize(Size);

	if (auto Image = Background.lock())
	{
		Image->SetSize(Size);
	}

	if (auto Image = SlotItem.lock())
	{
		Image->SetPos(Pad, Pad);
		Image->SetSize(Size.x - 2 * Pad, Size.y - 2 * Pad);
	}

	RefreshItemCountPosAndSize();
}

void CItemSlot::SetSize(float x, float y)
{
	SetSize(FVector(x, y, 0));
}

static const FColor& GetTierColor(int Tier)
{
	switch (Tier)
	{
	case 2:  return FColor::T2Color;
	case 3:  return FColor::T3Color;
	case 4:  return FColor::T4Color;
	default: return FColor::T1Color;
	}
}

void CItemSlot::SetItem(TableID ID, bool bWeapon) const
{
	if (ItemID == ID && bIsWeapon == bWeapon)
	{
		return;
	}

	auto Image = SlotItem.lock();
	if (!Image)
	{
		return;
	}

	if (bWeapon)
	{
		FWeaponInfo* Info;
		if (!WeaponTable::GetInst().TryGet(ID, Info))
		{
			Image->SetEnable(false);
			return;
		}

		if (Image->SetTexture(Info->IconPath, GetTextureFileNameTChar(Info->IconPath), Key::Path::Brotato))
		{
			Image->SetEnable(true);
		}

		if (auto BG = Background.lock())
		{
			BG->SetTint(GetTierColor(Info->Tier));
		}
	}
	else
	{
		FItemInfo* Info;
		if (!ItemTable::GetInst().TryGet(ID, Info))
		{
			Image->SetEnable(false);
			return;
		}

		if (Image->SetTexture(Info->IconPath, GetTextureFileNameTChar(Info->IconPath), Key::Path::Brotato))
		{
			Image->SetEnable(true);
		}

		if (auto BG = Background.lock())
		{
			BG->SetTint(GetTierColor(Info->Tier));
		}
	}
}

void CItemSlot::RefreshItemCountPosAndSize() const
{
	if (auto Text = ItemCount.lock())
	{
		Text->SetFontSize(FontSize);

		auto Size = FontSize * 1.5f;
		Text->SetSize(Size, Size);

		if (auto Slot = SlotItem.lock())
		{
			auto SlotPos = Slot->GetPos();
			auto SlotEnd = SlotPos + Slot->GetSize();
			auto TextPos = SlotEnd - Text->GetSize();
			TextPos.x -= Pad;
			TextPos.y -= Pad;

			Text->SetPos(TextPos);
		}
	}
}

void CItemSlot::SetFontSize(float NewSize)
{
	if (FontSize == NewSize)
	{
		return;
	}

	FontSize = NewSize;

	RefreshItemCountPosAndSize();
}

void CItemSlot::SetItemCount(int Count) const
{
	auto Text = ItemCount.lock();
	if (!Text)
	{
		return;
	}

	if (Count < 2)
	{
		Text->SetEnable(false);

		return;
	}

	Text->SetEnable(true);

	Text->SetText(TEXT("X"));
	Text->AddText(Count);
}

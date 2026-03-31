#include "CGoodsSlot.h"

#include <CDevice.h>
#include <UI/CButton.h>
#include <UI/CImage.h>
#include <UI/CTextBlock.h>
#include <UI/CWidgetContainer.h>
#include <World/CWorldUIManager.h>

#include "CItemSlot.h"
#include "CShopWidget.h"
#include "../Inventory/CCharacterData.h"
#include "../Inventory/CShop.h"
#include "../Strings.h"
#include "../Table/ItemInfo.h"
#include "../Table/ItemTable.h"
#include "../Table/WeaponInfo.h"
#include "../Table/WeaponTable.h"
#include "../Utility.h"

bool CGoodsSlot::Init()
{
	if (!CWidgetContainer::Init())
	{
		return false;
	}

	const float Ratio = CDevice::GetInst()->GetRatioFHD();

	const float SlotWidth  = 351.f * Ratio;
	const float SlotHeight = 483.f * Ratio;
	SetSize(FVector(SlotWidth, SlotHeight, 0.f));

	// ── Background ──────────────────────────────────────────────────
	Background = CreateWidget<CImage>("Background", 1);
	if (auto Image = Background.lock())
	{
		Image->SetSize(GetSize());
		Image->SetTint(FColor::HalfTransparent);
	}

	// ── ItemSlot (아이콘) ────────────────────────────────────────────
	const float ItemSlotPad   = 14.f  * Ratio;
	const float ItemSlotWidth = 100.f * Ratio;

	Slot = CreateWidget<CItemSlot>("Slot", 2);
	if (auto S = Slot.lock())
	{
		S->SetPos(ItemSlotPad, ItemSlotPad);
		S->SetSize(ItemSlotWidth, ItemSlotWidth);
	}

	// ── Name ─────────────────────────────────────────────────────────
	const float NameW        = 200.f * Ratio;
	const float NameH        = 30.f  * Ratio;
	const float NameFontSize = 26.f  * Ratio;

	Name = CreateWidget<CTextBlock>("Name", 2);
	if (auto Text = Name.lock())
	{
		if (auto S = Slot.lock())
		{
			auto Pos = S->GetPos();
			Pos.x += S->GetSize().x + ItemSlotPad;
			Text->SetPos(Pos);
		}
		Text->SetSize(FVector(NameW, NameH, 0.f));
		Text->SetAlignH(ETextAlignH::Left);
		Text->SetAlignV(ETextAlignV::Top);
		Text->SetFontSize(NameFontSize);
	}

	// ── BuyButton (중앙 하단) ────────────────────────────────────────
	const float BtnW    = 280.f * Ratio;
	const float BtnH    = 50.f  * Ratio;
	const float BtnPadB = 14.f  * Ratio;

	const float BtnX = (SlotWidth  - BtnW) * 0.5f;
	const float BtnY =  SlotHeight - BtnH  - BtnPadB;

	const float IconSize      = 32.f * Ratio;
	const float TextW         = BtnW - IconSize;
	const float PriceFontSize = 24.f * Ratio;

	BuyButton = CreateWidget<CButton>("BuyButton", 2);
	if (auto Button = BuyButton.lock())
	{
		Button->SetPos(BtnX, BtnY);
		Button->SetSize(BtnW, BtnH);
		Button->SetTint(EButtonState::Normal,  0.f,  0.f,  0.f,  0.6f);
		Button->SetTint(EButtonState::Hovered, 0.3f, 0.3f, 0.3f, 0.75f);
		Button->SetEventCallback<CGoodsSlot>(EButtonEventState::Click, this, &CGoodsSlot::OnClickBuy);

		// PriceText: 버튼 자식 (버튼 중앙에서 아이콘 절반만큼 왼쪽)
		if (auto Text = CWidget::CreateStaticWidget<CTextBlock>("PriceText", World, 3))
		{
			PriceText = Text;
			Text->SetPos(-(IconSize * 0.5f), 0.f);
			Text->SetSize(BtnW, BtnH);
			Text->SetAlignH(ETextAlignH::Center);
			Text->SetAlignV(ETextAlignV::Middle);
			Text->SetFontSize(PriceFontSize);
			Text->SetTextColor(FColor::White);
			Button->SetChild(Text);
		}
	}

	// PriceIcon: 슬롯에 직접 배치 (버튼 우측 위에 겹침, ZOrder 높게)
	PriceIcon = CreateWidget<CImage>("PriceIcon", 3);
	if (auto Icon = PriceIcon.lock())
	{
		Icon->SetTexture("harvesting_icon", TEXT("items/materials/harvesting_icon.png"), Key::Path::Brotato);
		Icon->SetPos(BtnX + TextW, BtnY + (BtnH - IconSize) * 0.5f);
		Icon->SetSize(IconSize, IconSize);
	}

	return true;
}

void CGoodsSlot::SetItem(const FShopGoods& InGoodsInfo)
{
	if (GoodsInfo.GoodsID == InGoodsInfo.GoodsID && GoodsInfo.bIsWeapon == InGoodsInfo.bIsWeapon)
	{
		return;
	}

	GoodsInfo = InGoodsInfo;

	if (auto S = Slot.lock())
	{
		S->SetItem(GoodsInfo.GoodsID, GoodsInfo.bIsWeapon);
	}

	if (GoodsInfo.bIsWeapon)
	{
		SetWeaponInfo();
	}
	else
	{
		SetItemInfo();
	}
}

// 가격에 따라 PriceText 색상을 설정하는 헬퍼
static void ApplyPriceColor(const std::weak_ptr<CTextBlock>& PriceText, int Price)
{
	if (auto Text = PriceText.lock())
	{
		const int MaterialCount = CCharacterData::GetInst().GetMaterialCount();
		if (Price > MaterialCount)
		{
			Text->SetTextColor(FColor(1.f, 0.f, 0.f, 1.f));
		}
		else
		{
			Text->SetTextColor(FColor::White);
		}
	}
}

void CGoodsSlot::SetItemInfo()
{
	FItemInfo* Info;
	if (!ItemTable::GetInst().TryGet(GoodsInfo.GoodsID, Info))
	{
		return;
	}

	if (auto Text = Name.lock())
	{
		Text->SetText(GetTextureFileNameTChar(Info->Name));
	}

	const int Price = GoodsInfo.GetPrice();

	if (auto Text = PriceText.lock())
	{
		Text->SetText(Price);
	}

	ApplyPriceColor(PriceText, Price);
}

void CGoodsSlot::SetWeaponInfo()
{
	FWeaponInfo* Info;
	if (!WeaponTable::GetInst().TryGet(GoodsInfo.GoodsID, Info))
	{
		return;
	}

	if (auto Text = Name.lock())
	{
		Text->SetText(GetTextureFileNameTChar(Info->Name));
	}

	const int Price = GoodsInfo.GetPrice();

	if (auto Text = PriceText.lock())
	{
		Text->SetText(Price);
	}

	ApplyPriceColor(PriceText, Price);
}

void CGoodsSlot::OnClickBuy()
{
	if (CCharacterData::GetInst().GetMaterialCount() < GoodsInfo.GetPrice())
	{
		return;
	}

	CCharacterData::GetInst().AddMaterialCount(-GoodsInfo.GetPrice());

	if (GoodsInfo.bIsWeapon)
	{
		CCharacterData::GetInst().AddWeapon(GoodsInfo);
	}
	else
	{
		CCharacterData::GetInst().AddItem(GoodsInfo);
	}

	SetEnable(false);

	if (auto Manager = UIManager.lock())
	{
		if (auto Shop = Manager->FindWidget<CShopWidget>("Shop").lock())
		{
			Shop->RefreshInventory();
		}
	}
}

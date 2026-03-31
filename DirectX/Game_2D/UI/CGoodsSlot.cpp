#include "CGoodsSlot.h"

#include <cmath>
#include <iomanip>
#include <sstream>

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
#include "../Table/MiscTable.h"
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

	const float EffectStartY = ItemSlotPad + ItemSlotWidth + 18.f * Ratio;
	const float EffectLineH = 28.f * Ratio;
	const float EffectIconSize = 24.f * Ratio;
	const float EffectValueW = 60.f * Ratio;
	const float EffectPad = 4.f * Ratio;
	const float EffectFontSize = 20.f * Ratio;
	const float WeaponLabelW = 118.f * Ratio;
	const float WeaponLineW = SlotWidth - ItemSlotPad * 2.f;

	ItemEffectLines.reserve(FItemInfo::MAX_EFFECT_COUNT);
	for (int i = 0; i < FItemInfo::MAX_EFFECT_COUNT; ++i)
	{
		FItemEffectLineWidgets Line;

		Line.Icon = CreateWidget<CImage>("EffectIcon", 2);
		if (auto Icon = Line.Icon.lock())
		{
			Icon->SetPos(ItemSlotPad, EffectStartY + EffectLineH * i);
			Icon->SetSize(EffectIconSize, EffectIconSize);
			Icon->SetEnable(false);
		}

		Line.Value = CreateWidget<CTextBlock>("EffectValue", 2);
		if (auto Text = Line.Value.lock())
		{
			Text->SetPos(ItemSlotPad + EffectIconSize + EffectPad, EffectStartY + EffectLineH * i);
			Text->SetSize(EffectValueW, EffectIconSize);
			Text->SetFontSize(EffectFontSize);
			Text->SetAlignH(ETextAlignH::Right);
			Text->SetAlignV(ETextAlignV::Middle);
			Text->SetEnable(false);
		}

		Line.Name = CreateWidget<CTextBlock>("EffectName", 2);
		if (auto Text = Line.Name.lock())
		{
			const float NameX = ItemSlotPad + EffectIconSize + EffectPad + EffectValueW + EffectPad;
			Text->SetPos(NameX, EffectStartY + EffectLineH * i);
			Text->SetSize(SlotWidth - NameX - ItemSlotPad, EffectIconSize);
			Text->SetFontSize(EffectFontSize);
			Text->SetAlignH(ETextAlignH::Left);
			Text->SetAlignV(ETextAlignV::Middle);
			Text->SetTextColor(FColor::White);
			Text->SetEnable(false);
		}

		ItemEffectLines.push_back(Line);
	}

	WeaponInfoLines.reserve(4);
	for (int i = 0; i < 4; ++i)
	{
		FWeaponInfoLineWidgets Line;

		Line.Label = CreateWidget<CTextBlock>("WeaponInfoLabel", 2);
		if (auto Text = Line.Label.lock())
		{
			Text->SetPos(ItemSlotPad, EffectStartY + EffectLineH * i);
			Text->SetSize(WeaponLabelW, EffectIconSize);
			Text->SetFontSize(EffectFontSize);
			Text->SetAlignH(ETextAlignH::Left);
			Text->SetAlignV(ETextAlignV::Middle);
			Text->SetTextColor(FColor(255.f / 255.f, 236.f / 255.f, 170.f / 255.f, 1.f));
			Text->SetEnable(false);
		}

		Line.Value = CreateWidget<CTextBlock>("WeaponInfoValue", 2);
		if (auto Text = Line.Value.lock())
		{
			Text->SetPos(ItemSlotPad + WeaponLabelW, EffectStartY + EffectLineH * i);
			Text->SetSize(WeaponLineW - WeaponLabelW, EffectIconSize);
			Text->SetFontSize(EffectFontSize);
			Text->SetAlignH(ETextAlignH::Left);
			Text->SetAlignV(ETextAlignV::Middle);
			Text->SetTextColor(FColor::White);
			Text->SetEnable(false);
		}

		WeaponInfoLines.push_back(Line);
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

static FColor GetTierColor(const int Tier)
{
	switch (Tier)
	{
	case 1:
		return FColor::White;
	case 2:
		return FColor(130.f / 255.f, 200.f / 255.f, 240.f / 255.f, 1.f);
	case 3:
		return FColor(190.f / 255.f, 130.f / 255.f, 255.f / 255.f, 1.f);
	case 4:
		return FColor(255.f / 255.f, 120.f / 255.f, 130.f / 255.f, 1.f);
	default:
		return FColor::White;
	}
}

static FColor GetStatValueColor(const int StatValue)
{
	if (StatValue > 0)
	{
		return FColor::Green;
	}

	if (StatValue < 0)
	{
		return FColor::Red;
	}

	return FColor::White;
}

static bool IsPrimaryStat(const EStat::Type StatType)
{
	return EStat::Level <= StatType && StatType <= EStat::Harvesting;
}

static std::wstring FormatWeaponCritValue(const FWeaponInfo& Info)
{
	std::wstringstream Stream;

	const float Multiplier = static_cast<float>(Info.CritDamagePercent) / 100.f;
	Stream << L"X";
	if (std::fabs(Multiplier - std::round(Multiplier)) < 0.01f)
	{
		Stream << static_cast<int>(std::round(Multiplier));
	}
	else
	{
		Stream << std::fixed << std::setprecision(2) << Multiplier;
	}

	Stream << L" (" << Info.CritChancePercent << L"% 확률)";
	return Stream.str();
}

static std::wstring FormatWeaponCooldownValue(const FWeaponInfo& Info)
{
	std::wstringstream Stream;
	const float CooldownSec = static_cast<float>(Info.CooldownMS) / 1000.f;
	Stream << std::fixed << std::setprecision(2) << CooldownSec << L"s";
	return Stream.str();
}

static std::wstring FormatWeaponRangeValue(const FWeaponInfo& Info)
{
	std::wstringstream Stream;
	Stream << Info.Range << L" (" << (Info.bIsMeleeWeapon ? L"근거리" : L"원거리") << L")";
	return Stream.str();
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
		Text->SetTextColor(GetTierColor(Info->Tier));
	}

	HideWeaponInfoLines();
	HideItemEffectLines();

	const auto Misc = MiscTable::GetInst().Get();
	if (Misc)
	{
		int LineIndex = 0;
		for (const auto& [StatType, StatValue] : Info->Effects)
		{
			if (LineIndex >= static_cast<int>(ItemEffectLines.size()))
			{
				break;
			}

			auto& Line = ItemEffectLines[LineIndex];

			EStat::Type IconStatType = StatType;
			if (!IsPrimaryStat(StatType))
			{
				IconStatType = EStat::None;
			}

			const auto IconIt = Misc->PrimaryStatIconPaths.find(IconStatType);
			if (IconIt != Misc->PrimaryStatIconPaths.end())
			{
				if (auto Icon = Line.Icon.lock())
				{
					const auto& IconPath = IconIt->second;
					std::string IconKey = "GoodsEffectIcon_" + std::to_string(static_cast<int>(IconStatType));
					Icon->SetTexture(IconKey, GetTextureFileNameTChar(IconPath), Key::Path::Brotato);
					Icon->SetEnable(true);
				}
			}

			if (auto ValueText = Line.Value.lock())
			{
				std::wstring ValueStr;
				if (StatValue > 0)
				{
					ValueStr = L"+";
					ValueStr += std::to_wstring(StatValue);
				}
				else
				{
					ValueStr = std::to_wstring(StatValue);
				}

				ValueText->SetText(ValueStr.c_str());
				ValueText->SetTextColor(GetStatValueColor(StatValue));
				ValueText->SetEnable(true);
			}

			if (auto NameText = Line.Name.lock())
			{
				NameText->SetText(GetStatName(StatType));
				NameText->SetTextColor(FColor::White);
				NameText->SetEnable(true);
			}

			++LineIndex;
		}
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

	HideItemEffectLines();
	HideWeaponInfoLines();

	if (auto Text = Name.lock())
	{
		Text->SetText(GetTextureFileNameTChar(Info->Name));
		Text->SetTextColor(GetTierColor(Info->Tier));
	}

	if (WeaponInfoLines.size() >= 4)
	{
		if (auto Label = WeaponInfoLines[0].Label.lock())
		{
			Label->SetText(TEXT("대미지:"));
			Label->SetEnable(true);
		}
		if (auto Value = WeaponInfoLines[0].Value.lock())
		{
			Value->SetText(Info->BaseDamage);
			Value->SetEnable(true);
		}

		if (auto Label = WeaponInfoLines[1].Label.lock())
		{
			Label->SetText(TEXT("치명타:"));
			Label->SetEnable(true);
		}
		if (auto Value = WeaponInfoLines[1].Value.lock())
		{
			auto CritText = FormatWeaponCritValue(*Info);
			Value->SetText(CritText.c_str());
			Value->SetEnable(true);
		}

		if (auto Label = WeaponInfoLines[2].Label.lock())
		{
			Label->SetText(TEXT("쿨다운:"));
			Label->SetEnable(true);
		}
		if (auto Value = WeaponInfoLines[2].Value.lock())
		{
			auto CooldownText = FormatWeaponCooldownValue(*Info);
			Value->SetText(CooldownText.c_str());
			Value->SetEnable(true);
		}

		if (auto Label = WeaponInfoLines[3].Label.lock())
		{
			Label->SetText(TEXT("범위:"));
			Label->SetEnable(true);
		}
		if (auto Value = WeaponInfoLines[3].Value.lock())
		{
			auto RangeText = FormatWeaponRangeValue(*Info);
			Value->SetText(RangeText.c_str());
			Value->SetEnable(true);
		}
	}

	const int Price = GoodsInfo.GetPrice();

	if (auto Text = PriceText.lock())
	{
		Text->SetText(Price);
	}

	ApplyPriceColor(PriceText, Price);
}

void CGoodsSlot::HideItemEffectLines()
{
	for (auto& Line : ItemEffectLines)
	{
		if (auto Icon = Line.Icon.lock())
		{
			Icon->SetEnable(false);
		}

		if (auto Value = Line.Value.lock())
		{
			Value->SetEnable(false);
		}

		if (auto NameText = Line.Name.lock())
		{
			NameText->SetEnable(false);
		}
	}
}

void CGoodsSlot::HideWeaponInfoLines()
{
	for (auto& Line : WeaponInfoLines)
	{
		if (auto Label = Line.Label.lock())
		{
			Label->SetEnable(false);
		}

		if (auto Value = Line.Value.lock())
		{
			Value->SetEnable(false);
		}
	}
}

void CGoodsSlot::OnClickBuy()
{
	auto& CharacterData = CCharacterData::GetInst();

	if (CharacterData.GetMaterialCount() < GoodsInfo.GetPrice())
	{
		return;
	}

	if (GoodsInfo.bIsWeapon && CharacterData.GetWeaponCount() >= INVENTORY_MAX_WEAPON)
	{
		return;
	}

	CharacterData.AddMaterialCount(-GoodsInfo.GetPrice());

	if (GoodsInfo.bIsWeapon)
	{
		CharacterData.AddWeapon(GoodsInfo);
	}
	else
	{
		CharacterData.AddItem(GoodsInfo);
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

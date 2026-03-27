#include "CStatValueWidget.h"

#include <UI/CButton.h>
#include <UI/CImage.h>
#include <CDevice.h>
#include <UI/CTextBlock.h>

#include "../Strings.h"
#include "../Utility.h"
#include "../Table/MiscTable.h"

bool CStatValueWidget::Init()
{
	if (!CWidgetContainer::Init())
	{
		return false;
	}

	auto RatioFHD = CDevice::GetInst()->GetRatioFHD();
	auto FontSize = 20 * RatioFHD;
	const FVector WidgetSize = FVector(375, 34, 0) * RatioFHD;
	SetSize(WidgetSize);

	TooltipButton = CreateWidget<CButton>("Button", 3);
	if (auto Button = TooltipButton.lock())
	{
		auto Size = FVector(375, 34, 0) * RatioFHD;
		Button->SetSize(Size);
		Button->SetPivot(0.5f, 0.5f);
		Button->SetEventCallback<CStatValueWidget>(EButtonEventState::Click, this, &CStatValueWidget::OnClickButton);
	}

	StatIcon = CreateWidget<CImage>("Icon", 4);
	if (auto Icon = StatIcon.lock())
	{
		if (auto Button = TooltipButton.lock())
		{
			Button->SetChild(Icon);
		}

		auto Size = FVector(34, 34, 0) * RatioFHD;
		Icon->SetSize(Size);

		auto Pad = 8 * RatioFHD;
		Icon->SetPos(Pad, 0);

		Icon->SetEnable(false);
	}

	const float Padding = 8.f * RatioFHD;
	const float CenterY = WidgetSize.y * 0.5f;
	const float ValueX = WidgetSize.x - Padding;

	StatName = CreateWidget<CTextBlock>("Name", 4);
	if (auto Text = StatName.lock())
	{
		Text->SetFontSize(FontSize);
		Text->SetAlignH(ETextAlignH::Left);
		Text->SetAlignV(ETextAlignV::Middle);

		Text->SetPos(ValueX, CenterY);
	}

	StatValue = CreateWidget<CTextBlock>("Value", 4);
	if (auto Text = StatValue.lock())
	{
		auto Size = FVector(100, 40, 0) * RatioFHD;
		Text->SetSize(Size);
		Text->SetFontSize(FontSize);
		Text->SetAlignH(ETextAlignH::Right);
		Text->SetAlignV(ETextAlignV::Middle);
		Text->SetPos(ValueX, CenterY);
	}

	SetPrimary(true);

	return true;
}

void CStatValueWidget::SetPrimary(const bool bIsPrimary)
{
	this->bIsPrimary = bIsPrimary;

	if (auto Icon = StatIcon.lock())
	{
		Icon->SetEnable(bIsPrimary);
	}

	auto TextName = StatName.lock();
	auto TextValue = StatValue.lock();
	auto Icon = StatIcon.lock();

	if (!TextName || !TextValue || !Icon)
	{
		return;
	}

	float IconLeft = 0.f;
	if (auto Button = TooltipButton.lock())
	{
		IconLeft += Button->GetPos().x;
	}

	IconLeft += Icon->GetPos().x - Icon->GetSize().x * Icon->GetPivot().x;
	const float IconRight = IconLeft + Icon->GetSize().x;

	const float ValueLeft = TextValue->GetPos().x - TextValue->GetSize().x * TextValue->GetPivot().x;
	const float NameLeft = bIsPrimary ? IconRight : IconLeft;
	const float NameWidth = ValueLeft > NameLeft ? (ValueLeft - NameLeft) : 0.f;

	auto NameSize = TextName->GetSize();
	NameSize.x = NameWidth;
	TextName->SetSize(NameSize);
	TextName->SetPos(ValueLeft, TextValue->GetPos().y);
}

void CStatValueWidget::SetStatType(const EStat::Type StatType)
{
	Type = StatType;

	if (auto Icon = StatIcon.lock())
	{
		auto Misc = MiscTable::GetInst().Get();
		const auto It = Misc->PrimaryStatIconPaths.find(StatType);
		if (It != Misc->PrimaryStatIconPaths.end())
		{
			std::wstring WideIconPath(It->second.begin(), It->second.end());
			Icon->SetTexture("harvesting_icon", WideIconPath.c_str(), Key::Path::Brotato);
		}
	}

	if (auto Text = StatName.lock())
	{
		Text->SetText(GetStatName(StatType));

		// TODO: 
	}
}

void CStatValueWidget::SetStatValue(const int NewValue)
{
	Value = NewValue;

	if (auto Text = StatValue.lock())
	{
		Text->SetText(NewValue);
	}
}

void CStatValueWidget::OnClickButton()
{
}

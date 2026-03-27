#include "CStatWidget.h"

#include <CDevice.h>
#include <UI/CButton.h>
#include <UI/CImage.h>
#include <UI/CTextBlock.h>

#include "CStatValueWidget.h"
#include "../Defines.h"
#include "../Inventory/CCharacterData.h"

bool CStatWidget::Init()
{
	if (!CWidgetContainer::Init())
	{
		return false;
	}

	auto RatioFHD = CDevice::GetInst()->GetRatioFHD();

	SetSize(FVector(400, 900, 0) * RatioFHD);

	Background = CreateWidget<CImage>("BG", 1);
	if (auto Image = Background.lock())
	{
		auto Size = FVector(400, 900, 0) * RatioFHD;
		Image->SetSize(Size);
		Image->SetPos(FVector::Zero);

		Image->SetTint(FColor::HalfTransparent);
	}

	Title = CreateWidget<CTextBlock>("Title", 2);
	if (auto Text = Title.lock())
	{
		Text->SetPos(0, 20 * RatioFHD);

		auto Size = FVector(400, 45, 0) * RatioFHD;
		Text->SetSize(Size);

		auto FontSize = 36 * RatioFHD;
		Text->SetFontSize(FontSize);
		Text->SetAlignH(ETextAlignH::Center);
		Text->SetAlignV(ETextAlignV::Top);

		Text->SetText(TEXT("능력치"));
	}

	PrimaryButton = CreateWidget<CButton>("PrimaryButton", 2);
	if (auto Button = PrimaryButton.lock())
	{
		auto Size = FVector(140, 50, 0) * RatioFHD;
		Button->SetSize(Size);

		auto Pos = FVector(50, 100, 0) * RatioFHD;
		Button->SetPos(Pos);

		Button->SetTint(EButtonState::Normal, 0.f, 0.f, 0.f, 0.6f);
		Button->SetTint(EButtonState::Hovered, 0.3f, 0.3f, 0.3f, 0.75f);

		Button->SetEventCallback<CStatWidget>(EButtonEventState::Click, this, &CStatWidget::PrintPrimary);
	}

	PrimaryText = CreateWidget<CTextBlock>("PrimaryButtonText", 3);
	if (auto Text = PrimaryText.lock())
	{
		if (auto Button = PrimaryButton.lock())
		{
			Button->SetChild(Text);
			Text->SetSize(Button->GetSize());
		}

		Text->SetPos(FVector::Zero);

		Text->SetAlignH(ETextAlignH::Center);
		Text->SetAlignV(ETextAlignV::Middle);

		auto FontSize = 30 * RatioFHD;
		Text->SetFontSize(FontSize);

		Text->SetTextColor(FColor::White);

		Text->SetText(TEXT("기본적인"));
	}

	SecondaryButton = CreateWidget<CButton>("SecondaryButton", 2);
	if (auto Button = SecondaryButton.lock())
	{
		auto Size = FVector(140, 50, 0) * RatioFHD;
		Button->SetSize(Size);

		auto Pos = FVector(210, 100, 0) * RatioFHD;
		Button->SetPos(Pos);

		Button->SetTint(EButtonState::Normal, 0.f, 0.f, 0.f, 0.6f);
		Button->SetTint(EButtonState::Hovered, 0.3f, 0.3f, 0.3f, 0.75f);

		Button->SetEventCallback<CStatWidget>(EButtonEventState::Click, this, &CStatWidget::PrintSecondary);
	}

	SecondaryText = CreateWidget<CTextBlock>("SecondaryButtonText", 3);
	if (auto Text = SecondaryText.lock())
	{
		if (auto Button = SecondaryButton.lock())
		{
			Button->SetChild(Text);
			Text->SetSize(Button->GetSize());
		}

		Text->SetPos(FVector::Zero);

		Text->SetAlignH(ETextAlignH::Center);
		Text->SetAlignV(ETextAlignV::Middle);

		auto FontSize = 30 * RatioFHD;
		Text->SetFontSize(FontSize);

		Text->SetTextColor(FColor::White);

		Text->SetText(TEXT("2차적인"));
	}

	Stats.reserve(EStat::EnemySpeed - EStat::ConsumableHeal + 1);
	auto OffsetY = 180 * RatioFHD;
	for (int i = EStat::ConsumableHeal; i <= EStat::EnemySpeed; i++)
	{
		if (auto Item = CreateWidget<CStatValueWidget>("StatItem", 2).lock())
		{
			Item->SetPos(20, OffsetY + (i - EStat::ConsumableHeal) * Item->GetSize().y);

			Stats.push_back(Item);
		}
	}

	PrintPrimary();

	return true;
}

void CStatWidget::PrintPrimary()
{
	auto Active = PrimaryButton.lock();
	auto Inactive = SecondaryButton.lock();

	Active->SetTint(EButtonState::Normal, 0.3f, 0.3f, 0.3f, 0.75f);
	Inactive->SetTint(EButtonState::Normal, FColor::HalfTransparent);

	auto StatCount = EStat::Harvesting - EStat::Level + 1;
	int i = 0;
	for (; i < StatCount; i++)
	{
		auto Item = Stats[i].lock();
		auto Stat = static_cast<EStat::Type>(i + EStat::Level);

		Item->SetStat(Stat, static_cast<int>(std::round(CCharacterData::GetInst().GetStat(Stat))), true);

		Item->SetEnable(true);
	}

	for (; i < Stats.size(); i++)
	{
		auto Item = Stats[i].lock();
		auto Stat = static_cast<EStat::Type>(i + EStat::Level);

		Item->SetEnable(false);
	}
}

void CStatWidget::PrintSecondary()
{
	auto Active = SecondaryButton.lock();
	auto Inactive = PrimaryButton.lock();

	Active->SetTint(EButtonState::Normal, 0.3f, 0.3f, 0.3f, 0.75f);
	Inactive->SetTint(EButtonState::Normal, FColor::HalfTransparent);

	auto StatCount = EStat::EnemySpeed - EStat::ConsumableHeal + 1;
	int i = 0;
	for (; i < StatCount; i++)
	{
		auto Item = Stats[i].lock();
		auto Stat = static_cast<EStat::Type>(i + EStat::ConsumableHeal);

		Item->SetStat(Stat, static_cast<int>(std::round(CCharacterData::GetInst().GetStat(Stat))), false);

		Item->SetEnable(true);
	}

	for (; i < Stats.size(); i++)
	{
		auto Item = Stats[i].lock();
		auto Stat = static_cast<EStat::Type>(i + EStat::ConsumableHeal);

		Item->SetEnable(false);
	}
}

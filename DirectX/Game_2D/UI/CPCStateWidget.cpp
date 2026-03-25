#include "CPCStateWidget.h"

#include <CDevice.h>
#include <UI/CImage.h>
#include <UI/CTextBlock.h>
#include <UI/CWidgetContainer.h>

#include "CBrotatoProgressBar.h"
#include "../Strings.h"
#include "../Character/CPlayerCharacter.h"
#include "../Inventory/CCharacterData.h"

void CPCStateWidget::OnPlayerHPChanged(float CurrentHP, float MaxHP)
{
	if (auto Bar = HpBar.lock())
	{
		Bar->SetHPValue(CurrentHP, MaxHP);
	}
}

void CPCStateWidget::SetPlayerCharacter(const std::weak_ptr<CPlayerCharacter>& InPlayerCharacter)
{
	PlayerCharacter = InPlayerCharacter;

	if (auto PC = PlayerCharacter.lock())
	{
		const float MaxHP = PC->GetStat(EStat::MaxHP);
		OnPlayerHPChanged(MaxHP, MaxHP);
	}
}

bool CPCStateWidget::Init()
{
	if (!CWidgetContainer::Init())
	{
		return false;
	}

	const float RatioFhd = CDevice::GetInst()->GetRatioFHD();
	const FVector2 BarSize = FVector2(320, 48) * RatioFhd;

	HpBar = CreateWidget<CBrotatoProgressBar>("HpBar");
	if (auto Bar = HpBar.lock())
	{
		Bar->SetPos(0, 0);
		Bar->SetSize(BarSize);
		Bar->SetBarType(EBrotatoProgressBarType::HP);
		Bar->SetTextColor(FColor::White);
		Bar->SetRatio(1.f);
	}

	ExpBar = CreateWidget<CBrotatoProgressBar>("ExpBar");
	if (auto Bar = ExpBar.lock())
	{
		Bar->SetPos(0, 60 * RatioFhd);
		Bar->SetSize(BarSize);
		Bar->SetBarType(EBrotatoProgressBarType::EXP);
		Bar->SetTextColor(FColor::White);
		Bar->SetRatio(0.f);
	}

	MaterialIcon = CreateWidget<CImage>("MaterialIcon");
	if (auto Icon = MaterialIcon.lock())
	{
		Icon->SetPivot(0, 0.5f);
		Icon->SetSize(FVector2(80, 80) * RatioFhd);
		Icon->SetPos(0, 150 * RatioFhd);
		Icon->SetTexture("harvesting_icon", TEXT("items/materials/harvesting_icon.png"), Key::Path::Brotato);
	}

	MaterialCount = CreateWidget<CTextBlock>("MaterialCount");
	if (auto Text = MaterialCount.lock())
	{
		const auto Size = FVector(300, 60, 0) * RatioFhd;
		const auto Pos = FVector(75, 120, 0) * RatioFhd;

		Text->SetPivot(0, 0.5f);
		Text->SetSize(Size);
		Text->SetPos(Pos);

		Text->SetFontSize(48 * RatioFhd);
		Text->SetTextColor(FColor::White);
		Text->SetAlignH(ETextAlignH::Left);
		Text->SetAlignV(ETextAlignV::Middle);

		Text->SetText(0);
	}

	return true;
}

void CPCStateWidget::Update(const float DeltaTime)
{
	CWidgetContainer::Update(DeltaTime);

	if (auto PC = PlayerCharacter.lock())
	{
		const float CurrentHP = PC->GetCurrHP();
		const float MaxHP = PC->GetStat(EStat::MaxHP);

		if (auto Bar = HpBar.lock())
		{
			Bar->SetHPValue(CurrentHP, MaxHP);
		}
	}

	if (auto Bar = ExpBar.lock())
	{
		int Level = CCharacterData::GetInst().GetLevel();
		int NeedExp = CCharacterData::GetLevelUpEXP(Level + 1);
		int CurrExp = CCharacterData::GetInst().GetExp();
		Bar->SetExpValue(CurrExp, NeedExp, Level);
	}

	if (auto Text = MaterialCount.lock())
	{
		Text->SetText(CCharacterData::GetInst().GetMaterialCount());
	}
}

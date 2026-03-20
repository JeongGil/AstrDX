#include "CPCStateWidget.h"

#include <CDevice.h>
#include <UI/CImage.h>
#include <UI/CTextBlock.h>
#include <UI/CWidgetContainer.h>

#include "CBrotatoProgressBar.h"
#include "../Strings.h"
#include "../Character/CPlayerCharacter.h"
#include "../Inventory/CCharacterData.h"

CPCStateWidget::CPCStateWidget()
{
}

CPCStateWidget::~CPCStateWidget()
{
}

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

	const FVector2 Size = FVector2(320, 48) * CDevice::GetInst()->GetRatioFHD();

	HpBar = CreateWidget<CBrotatoProgressBar>("HpBar");
	if (auto Bar = HpBar.lock())
	{
		Bar->SetPos(0, 0);
		Bar->SetSize(Size);
		Bar->SetBarType(EBrotatoProgressBarType::HP);
		Bar->SetTextColor(FColor::White);
		Bar->SetRatio(1.f);
	}

	ExpBar = CreateWidget<CBrotatoProgressBar>("ExpBar");
	if (auto Bar = ExpBar.lock())
	{
		Bar->SetPos(0, 40);
		Bar->SetSize(Size);
		Bar->SetBarType(EBrotatoProgressBarType::EXP);
		Bar->SetTextColor(FColor::White);
		Bar->SetRatio(0.f);
	}

	MaterialIcon = CreateWidget<CImage>("MaterialIcon");
	if (auto Icon = MaterialIcon.lock())
	{
		Icon->SetPivot(0, 0.5f);
		Icon->SetSize(FVector2(80, 80) * CDevice::GetInst()->GetRatioFHD());
		Icon->SetPos(0, 100);
		Icon->SetTexture("harvesting_icon", TEXT("items/materials/harvesting_icon.png"), Key::Path::Brotato);
	}

	MaterialCount = CreateWidget<CTextBlock>("MaterialCount");
	if (auto Text = MaterialCount.lock())
	{
		Text->SetPivot(0, 0.5f);
		Text->SetSize(200, 40);
		Text->SetPos(50, 80);

		Text->SetFontSize(32);
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
		//Bar->SetEXPValue()
	}

	if (auto Text = MaterialCount.lock())
	{
		Text->SetText(CCharacterData::GetInst().GetMaterialCount());
	}
}

void CPCStateWidget::Render()
{
	CWidgetContainer::Render();
}

#include "CPCStateWidget.h"

#include "CBrotatoProgressBar.h"
#include <UI/CWidgetContainer.h>
#include "../Character/CPlayerCharacter.h"

CPCStateWidget::CPCStateWidget()
{
}

CPCStateWidget::~CPCStateWidget()
{
}

void CPCStateWidget::OnPlayerHPChanged(float CurrentHP, float MaxHP)
{
	if (auto Bar = HPBar.lock())
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

	HPBar = CreateWidget<CBrotatoProgressBar>("HPBar");
	if (auto Bar = HPBar.lock())
	{
		Bar->SetPos(0.f, 0.f);
		Bar->SetSize(320.f, 48.f);
		Bar->SetBarType(EBrotatoProgressBarType::HP);
		Bar->SetTextColor(255, 255, 255, 255);
		Bar->SetRatio(1.f);
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
		
		if (auto Bar = HPBar.lock())
		{
			Bar->SetHPValue(CurrentHP, MaxHP);
		}
	}
}

void CPCStateWidget::Render()
{
	CWidgetContainer::Render();
}

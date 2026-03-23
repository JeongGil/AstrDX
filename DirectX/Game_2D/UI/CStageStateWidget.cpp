#include "CStageStateWidget.h"

#include <CDevice.h>
#include <UI/CTextBlock.h>

#include "../World/CBrotatoWorld_Battle.h"

bool CStageStateWidget::Init()
{
	if (!CWidgetContainer::Init())
	{
		return false;
	}

	StageLevel = CreateWidget<CTextBlock>("Level");
	if (auto Text = StageLevel.lock())
	{
		const FVector2 Size = FVector2(320, 60);
		Text->SetSize(Size);

		Text->SetPos(-Size.x * 0.5f, 0.f);

		Text->SetFontSize(48);
		Text->SetTextColor(FColor::White);
		Text->SetAlignH(ETextAlignH::Center);
		Text->SetAlignV(ETextAlignV::Top);

		Text->SetText(TEXT("웨이브 "));
		if (auto World = std::dynamic_pointer_cast<CBrotatoWorld_Battle>(this->World.lock()))
		{
			Text->AddText(World->GetStageLevel());
		}
	}

	RemainTime = CreateWidget<CTextBlock>("Time");
	if (auto Text = RemainTime.lock())
	{
		const FVector2 Size = FVector2(320, 60);
		Text->SetSize(Size);

		Text->SetPos(-Size.x * 0.5f, 60.f);

		Text->SetFontSize(36);
		Text->SetTextColor(FColor::White);
		Text->SetAlignH(ETextAlignH::Center);
		Text->SetAlignV(ETextAlignV::Top);
	}

	return true;
}

void CStageStateWidget::Update(const float DeltaTime)
{
	CWidgetContainer::Update(DeltaTime);

	if (auto Text = RemainTime.lock())
	{
		if (auto World = std::dynamic_pointer_cast<CBrotatoWorld_Battle>(this->World.lock()))
		{
			auto Time = static_cast<int>(std::ceil(World->GetRemainStageTime()));
			Text->SetText(Time);
		}
	}
}

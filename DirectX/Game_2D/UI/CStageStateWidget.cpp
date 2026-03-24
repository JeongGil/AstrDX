#include "CStageStateWidget.h"

#include <CDevice.h>
#include <UI/CImage.h>
#include <UI/CTextBlock.h>

#include "../Inventory/CCharacterData.h"
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
		Text->AddText(CCharacterData::GetInst().GetStageLevel());
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

	StageResult = CreateWidget<CTextBlock>("StageResult");
	if (auto Text = StageResult.lock())
	{
		const FVector2 Size = FVector2(420.f, 90.f);
		Text->SetSize(Size);

		Text->SetPos(-Size.x * 0.5f, 120.f);

		Text->SetFontSize(60.f);
		Text->SetTextColor(FColor::White);
		Text->SetAlignH(ETextAlignH::Center);
		Text->SetAlignV(ETextAlignV::Top);

		Text->SetEnable(false);
	}

	ResultBG = CreateWidget<CImage>("BG", -1);
	if (auto BG = ResultBG.lock())
	{
		BG->SetWidgetColor(0.3f, 0.3f, 0.3f, 0.3f);

		BG->SetEnable(false);
	}

	return true;
}

void CStageStateWidget::Update(const float DeltaTime)
{
	CWidgetContainer::Update(DeltaTime);

	switch (CCharacterData::GetInst().GetStageState())
	{
		case EStageState::Playing:
		{
			if (auto Text = RemainTime.lock())
			{
				if (auto World = std::dynamic_pointer_cast<CBrotatoWorld_Battle>(this->World.lock()))
				{
					auto Time = max(0, static_cast<int>(std::ceil(World->GetRemainStageTime())));
					Text->SetText(Time);

					if (Time == 0)
					{
						Text->SetTextColor(FColor::Red);
					}
				}
			}

			break;
		}

		case EStageState::Clear:
		case EStageState::Defeat:
		{
			if (auto Image = ResultBG.lock())
			{
				Image->SetEnable(true);
			}

			break;
		}

	case EStageState::None:
	default:
		break;
	}
}

void CStageStateWidget::SetEnableStageResult(bool bEnable) const
{
	if (auto Comp = StageResult.lock())
	{
		Comp->SetEnable(bEnable);
	}
}

void CStageStateWidget::SetStageResultText(bool bClear) const
{
	if (auto Text = StageResult.lock())
	{
		Text->SetText(bClear ? TEXT("웨이브 완료") : TEXT("달리기 패배"));
	}
}

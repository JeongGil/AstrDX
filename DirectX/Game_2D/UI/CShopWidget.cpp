#include "CShopWidget.h"

#include <CDevice.h>
#include <UI/CButton.h>
#include <UI/CImage.h>
#include <UI/CTextBlock.h>

#include "CGoodsWidget.h"
#include "CItemInvenWidget.h"
#include "CStatWidget.h"
#include "CWeaponInvenWidget.h"
#include "../Inventory/CCharacterData.h"

bool CShopWidget::Init()
{
	if (!CWidgetContainer::Init())
	{
		return false;
	}

	Background = CreateWidget<CImage>("BG");
	if (auto BG = Background.lock())
	{
		// CResultWidget::Init에서 이미 텍스쳐 로드.
		BG->SetTexture("ShopBG");

		auto RS = CDevice::GetInst()->GetResolution();
		BG->SetSize(RS.Width, RS.Height);
	}

	Goods = CreateWidget<CGoodsWidget>("Goods", 1);
	Stats = CreateWidget<CStatWidget>("Stats", 1);
	Items = CreateWidget<CItemInvenWidget>("Items", 1);
	Weapons = CreateWidget<CWeaponInvenWidget>("Weapons", 1);

	auto RS = CDevice::GetInst()->GetResolution();
	float Ratio = CDevice::GetInst()->GetRatioFHD();

	// Title: 좌측 상단
	Title = CreateWidget<CTextBlock>("Title", 2);
	if (auto TitleText = Title.lock())
	{
		FVector2 TitlePos = FVector2(36, 36) * Ratio;
		TitleText->SetPos(TitlePos);
		TitleText->SetPivot(0, 0);
		TitleText->SetSize(500.f * Ratio, 80.f * Ratio);
		TitleText->SetText(TEXT("상점 (웨이브 1)"));
		TitleText->SetAlignH(ETextAlignH::Left);
		TitleText->SetAlignV(ETextAlignV::Top);
		TitleText->SetFontSize(54 * Ratio);
		TitleText->SetTextColor(FColor::White);
	}

	// PlayButton: 우측 하단
	FVector2 ButtonSize = FVector2(400.f, 90.f) * Ratio;
	PlayButton = CreateWidget<CButton>("PlayButton", 1);
	if (auto Button = PlayButton.lock())
	{
		FVector2 ButtonPos = FVector2(RS.Width - 36 * Ratio, RS.Height - 36 * Ratio);

		Button->SetPivot(1, 1);
		Button->SetPos(ButtonPos);
		Button->SetSize(ButtonSize);
		Button->SetTint(EButtonState::Normal, 0, 0, 0, 0.6f);
		Button->SetTint(EButtonState::Hovered, 0.3f, 0.3f, 0.3f, 0.75f);

		if (auto Text = CWidget::CreateStaticWidget<CTextBlock>("PlayText", World, 2))
		{
			Playtext = Text;
			Text->SetPos(-ButtonSize.x, -ButtonSize.y);
			Text->SetSize(ButtonSize);
			Text->SetText(TEXT("이동 (웨이브 "));
			Text->AddText(CCharacterData::GetInst().GetStageLevel() + 1);
			Text->AddText(TEXT(")"));
			Text->SetAlignH(ETextAlignH::Center);
			Text->SetAlignV(ETextAlignV::Middle);
			Text->SetFontSize(45 * Ratio);
			Text->SetTextColor(FColor::White);
			Button->SetChild(Text);
		}
	}

	return true;
}

void CShopWidget::SetCurrentLevel(const int NewLevel)
{
	if (CurrentLevel == NewLevel)
	{
		return;
	}

	CurrentLevel = NewLevel;

	if (auto Text = Title.lock())
	{
		Text->SetText(TEXT("상점 (웨이브 "));
		Text->AddText(GetCurrentLevel());
		Text->AddText(TEXT(")"));
	}

	if (auto Text = Playtext.lock())
	{
		Text->SetText(TEXT("이동 (웨이브 "));
		Text->AddText(GetNextLevel());
		Text->AddText(TEXT(")"));
	}
}

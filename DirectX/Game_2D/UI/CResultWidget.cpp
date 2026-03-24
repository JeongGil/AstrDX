#include "CResultWidget.h"

#include <UI/CTextBlock.h>
#include <CDevice.h>
#include <UI/CImage.h>
#include <UI/CButton.h>
#include <World/CWorldManager.h>

#include "../Strings.h"
#include "../Inventory/CCharacterData.h"
#include "../World/CLoadingWorld.h"

bool CResultWidget::Init()
{
	if (!CWidgetContainer::Init())
	{
		return false;
	}

	auto RS = CDevice::GetInst()->GetResolution();

	Background = CreateWidget<CImage>("Background");
	if (auto BG = Background.lock())
	{
		BG->SetSize(RS.Width, RS.Height);
		BG->SetZOrder(0);

		BG->SetTexture("ShopBG", TEXT("ui/menus/shop/shop_background.png"), Key::Path::Brotato);
	}

	StatWidget;
	WeaponWidget;
	ItemWidget;

	const float RatioFHD = CDevice::GetInst()->GetRatioFHD();
	FVector2 ButtonSize = FVector2(260.f, 90.f) * RatioFHD;
	const float Margin = 36.f * RatioFHD;

	RestartButton = CreateWidget<CButton>("RestartButton", 1);
	if (auto Button = RestartButton.lock())
	{
		Button->SetPivot(0.f, 1.f);
		Button->SetPos(Margin, static_cast<float>(RS.Height) - Margin);
		Button->SetSize(ButtonSize);

		if (auto Text = CWidget::CreateStaticWidget<CTextBlock>("RestartText", World))
		{
			Text->SetPos(0.f, -ButtonSize.y);
			Text->SetText(TEXT("재시작"));
			Text->SetAlignH(ETextAlignH::Center);
			Text->SetAlignV(ETextAlignV::Middle);
			Text->SetFontSize(36.f);
			Text->SetTextColor(255, 255, 255, 255);

			Button->SetChild(Text);
		}

		Button->SetTint(EButtonState::Normal, 0.f, 0.f, 0.f, 0.6f);
		Button->SetTint(EButtonState::Hovered, 0.3f, 0.3f, 0.3f, 0.75f);

		Button->SetEventCallback<CResultWidget>(EButtonEventState::Click, this, &CResultWidget::OnClickRestart);
	}

	TitleButton = CreateWidget<CButton>("TitleButton", 1);
	if (auto Button = TitleButton.lock())
	{
		Button->SetPivot(1.f, 1.f);
		Button->SetPos(static_cast<float>(RS.Width) - Margin, static_cast<float>(RS.Height) - Margin);
		ButtonSize.x *= 2.5f;
		Button->SetSize(ButtonSize);

		if (auto Text = CWidget::CreateStaticWidget<CTextBlock>("TitleText", World))
		{
			Text->SetPos(-ButtonSize.x, -ButtonSize.y);
			Text->SetText(TEXT("메인 메뉴로 돌아가기"));
			Text->SetAlignH(ETextAlignH::Center);
			Text->SetAlignV(ETextAlignV::Middle);
			Text->SetFontSize(36.f);
			Text->SetTextColor(255, 255, 255, 255);

			Button->SetChild(Text);
		}

		Button->SetTint(EButtonState::Normal, 0.f, 0.f, 0.f, 0.6f);
		Button->SetTint(EButtonState::Hovered, 0.3f, 0.3f, 0.3f, 0.75f);

		Button->SetEventCallback<CResultWidget>(EButtonEventState::Click, this, &CResultWidget::OnClickTitle);
	}

	return true;
}

void CResultWidget::OnClickRestart()
{
	CCharacterData::GetInst().Init();

	if (auto World = CWorldManager::GetInst()->CreateWorld<CLoadingWorld>(true).lock())
	{
		World->Load(EWorldType::Main);
	}
}

void CResultWidget::OnClickTitle()
{
	CCharacterData::GetInst().Init();

	if (auto World = CWorldManager::GetInst()->CreateWorld<CLoadingWorld>(true).lock())
	{
		World->Load(EWorldType::Start);
	}
}

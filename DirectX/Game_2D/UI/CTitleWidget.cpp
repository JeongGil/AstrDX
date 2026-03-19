#include "CTitleWidget.h"

#include <CDevice.h>
#include <CEngine.h>
#include <UI/CButton.h>
#include <UI/CImage.h>
#include <UI/CTextBlock.h>
#include <World/CWorldManager.h>

#include "../Strings.h"
#include "../World/CLoadingWorld.h"

bool CTitleWidget::Init()
{
	if (!CWidgetContainer::Init())
	{
		return false;
	}

	auto RS = CDevice::GetInst()->GetResolution();
	const auto Center = FVector2(RS.Width * 0.5f, RS.Height * 0.5f);

#pragma region Background
	FVector2 BgSize = FVector2(2070, 1080) * CDevice::GetInst()->GetRatioFHD();

	if (auto BG = CreateWidget<CImage>("BG", 0).lock())
	{
		BG->SetSize(BgSize);
		BG->SetPos(Center);
		BG->SetPivot(0.5f, 0.5f);
		BG->SetTexture("TitleBG_BG", TEXT("ui/menus/title_screen/title_screen_background/splash_art_bg.png"), Key::Path::Brotato);
	}

	if (auto MistBack = CreateWidget<CImage>("MistBack", 1).lock())
	{
		MistBack->SetSize(BgSize);
		MistBack->SetPos(Center);
		MistBack->SetPivot(0.5f, 0.5f);
		MistBack->SetTexture("TitleBG_MistBack", TEXT("ui/menus/title_screen/title_screen_background/splash_art_mist_back.png"), Key::Path::Brotato);
	}

	if (auto MistMid = CreateWidget<CImage>("MistMid", 2).lock())
	{
		MistMid->SetSize(BgSize);
		MistMid->SetPos(Center);
		MistMid->SetPivot(0.5f, 0.5f);
		MistMid->SetTexture("TitleBG_MistMid", TEXT("ui/menus/title_screen/title_screen_background/splash_art_mist_mid.png"), Key::Path::Brotato);
	}

	if (auto Brotato = CreateWidget<CImage>("Brotato", 3).lock())
	{
		Brotato->SetSize(BgSize);
		Brotato->SetPos(Center);
		Brotato->SetPivot(0.5f, 0.5f);
		Brotato->SetTexture("TitleBG_Brotato", TEXT("ui/menus/title_screen/title_screen_background/splash_art_brotato.png"), Key::Path::Brotato);
	}

	if (auto MistFront = CreateWidget<CImage>("MistFront", 4).lock())
	{
		MistFront->SetSize(BgSize);
		MistFront->SetPos(Center);
		MistFront->SetPivot(0.5f, 0.5f);
		MistFront->SetTexture("TitleBG_MistFront", TEXT("ui/menus/title_screen/title_screen_background/splash_art_mist_front.png"), Key::Path::Brotato);
	}

	if (auto PostProcess = CreateWidget<CImage>("PostProcess", 5).lock())
	{
		PostProcess->SetSize(BgSize);
		PostProcess->SetPos(Center);
		PostProcess->SetPivot(0.5f, 0.5f);
		PostProcess->SetTexture("TitleBG_PostProcess", TEXT("ui/menus/title_screen/title_screen_background/splash_art_post_processing.png"), Key::Path::Brotato);
	}
#pragma endregion

#pragma region Button
	FVector2 ButtonSize = FVector2(260, 90) * CDevice::GetInst()->GetRatioFHD();
	if (auto Button = CreateWidget<CButton>("StartButton", 10).lock())
	{
		FVector2 Pos = FVector2(36, 660) * CDevice::GetInst()->GetRatioFHD();

		Button->SetPivot(0, 0.5f);
		Button->SetPos(Pos);
		Button->SetSize(ButtonSize);

		if (auto Text = CWidget::CreateStaticWidget<CTextBlock>("StartButtonText", World))
		{
			Text->SetPos(0.f, -ButtonSize.y * 0.5f);
			Text->SetText(TEXT("시작"));
			Text->SetAlignH(ETextAlignH::Center);
			Text->SetAlignV(ETextAlignV::Middle);
			Text->SetFontSize(36.f);
			Text->SetTextColor(255, 255, 255, 255);

			Button->SetChild(Text);
		}

		Button->SetTint(EButtonState::Normal, 0, 0, 0, 0.6f);
		Button->SetTint(EButtonState::Hovered, 0.3f, 0.3f, 0.3f, 0.75f);

		Button->SetEventCallback<CTitleWidget>(EButtonEventState::Click, this, &CTitleWidget::OnClickStart);
	}

	if (auto Button = CreateWidget<CButton>("ExitButton", 10).lock())
	{
		FVector2 Pos = FVector2(36, 770) * CDevice::GetInst()->GetRatioFHD();

		Button->SetPivot(0, 0.5f);
		Button->SetPos(Pos);
		Button->SetSize(ButtonSize);

		if (auto Text = CWidget::CreateStaticWidget<CTextBlock>("ExitButtonText", World))
		{
			Text->SetPos(0.f, -ButtonSize.y * 0.5f);
			Text->SetText(TEXT("종료"));
			Text->SetAlignH(ETextAlignH::Center);
			Text->SetAlignV(ETextAlignV::Middle);
			Text->SetFontSize(36.f);
			Text->SetTextColor(255, 255, 255, 255);

			Button->SetChild(Text);
		}

		Button->SetTint(EButtonState::Normal, 0, 0, 0, 0.6f);
		Button->SetTint(EButtonState::Hovered, 0.3f, 0.3f, 0.3f, 0.75f);

		Button->SetEventCallback<CTitleWidget>(EButtonEventState::Click, this, &CTitleWidget::OnClickExit);
	}
#pragma endregion 

	return true;
}

void CTitleWidget::OnClickStart()
{
	if (auto World = CWorldManager::GetInst()->CreateWorld<CLoadingWorld>(true).lock())
	{
		World->Load(EWorldType::Main);
	}
}

void CTitleWidget::OnClickExit()
{
	CEngine::GetInst()->Destroy();
}

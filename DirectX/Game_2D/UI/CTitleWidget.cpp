#include "CTitleWidget.h"

#include <UI/CButton.h>
#include <UI/CImage.h>
#include <CDevice.h>

#include "../Strings.h"

bool CTitleWidget::Init()
{
	if (!CWidgetContainer::Init())
	{
		return false;
	}

	auto RS = CDevice::GetInst()->GetResolution();
	const auto Center = FVector2(RS.Width * 0.5f, RS.Height * 0.5f);

#pragma region Background
	FVector2 BgSize = { 2070.f, 1080.f };
	BgSize *= RS.Height / BgSize.y;

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
	if (auto StartButton = CreateWidget<CButton>("StartButton").lock())
	{

	}
#pragma endregion 

	return true;
}

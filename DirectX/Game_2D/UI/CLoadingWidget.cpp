#include "CLoadingWidget.h"

#include <UI/CImage.h>
#include <UI/CTextBlock.h>
#include <CDevice.h>

CLoadingWidget::CLoadingWidget()
{
}

CLoadingWidget::~CLoadingWidget()
{
}

bool CLoadingWidget::Init()
{
	if (!CWidgetContainer::Init())
	{
		return false;
	}

	FResolution	RS = CDevice::GetInst()->GetResolution();
	auto Ratio = CDevice::GetInst()->GetRatioFHD();

	if (auto Back = CreateWidget<CImage>("Back").lock())
	{
		Back->SetSize((float)RS.Width, (float)RS.Height);
		Back->SetTint(FColor(28 / 255.f, 28 / 255.f, 28 / 255.f, 1));
	}

	if (auto Body = CreateWidget<CImage>("Body", 1).lock())
	{
		auto Size = FVector(245, 300, 0) * Ratio;
		Body->SetSize(Size);
		Body->SetPos(0.f, (float)RS.Height - Size.y);
		Body->SetTexture("LoadingBody", TEXT("Brotato\\ui\\splash\\splash_anim_evilboy_0008.png"), "Asset");
	}

	if (auto Title = CreateWidget<CImage>("Title", 1).lock())
	{
		auto Size = FVector(245, 300, 0) * Ratio;
		Title->SetSize(Size);
		Title->SetPos(0.f, (float)RS.Height - Size.y);
		Title->SetTexture("LoadingTitle", TEXT("Brotato\\ui\\splash\\splash_anim_eetitle_0006.png"), "Asset");
	}


	if (auto Fish = CreateWidget<CImage>("Fish", 1).lock())
	{
		auto Size = FVector(720, 590, 0) * Ratio;
		Fish->SetSize(Size);
		Fish->SetPos(((float)RS.Width - Size.x) * 0.5f, ((float)RS.Height - Size.y) * 0.5f);
		Fish->SetTexture("LoadingFish", TEXT("Brotato\\ui\\splash\\splash_anim_blobfish_0.png"), "Asset");
	}

	return true;
}

void CLoadingWidget::Update(const float DeltaTime)
{
	CWidgetContainer::Update(DeltaTime);
}

void CLoadingWidget::Render()
{
	CWidgetContainer::Render();
}

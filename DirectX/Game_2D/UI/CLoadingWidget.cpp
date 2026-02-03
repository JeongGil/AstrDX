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

	if (auto Back = CreateWidget<CImage>("Back").lock())
	{
		Back->SetSize((float)RS.Width, (float)RS.Height);
		Back->SetTexture("LoadingBack", TEXT("LoadingBack.png"));
	}

	if (auto Text = CreateWidget<CTextBlock>("Text", 1).lock())
	{
		Text->SetText(TEXT("LOADING"));
		Text->SetTextColor(255, 255, 255, 255);
		Text->SetPos(900.f, 550.f);
		Text->SetSize(500.f, 150.f);
		Text->SetFontSize(50.f);
		Text->EnableShadow(true);
		Text->SetShadowOffset(3.f, 3.f);
		Text->SetShadowTextColor(128, 128, 128, 255);
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

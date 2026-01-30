#include "CWorldHUD.h"

#include <UI/CTextBlock.h>
#include <UI/CProgressBar.h>

CWorldHUD::CWorldHUD()
{
}

CWorldHUD::~CWorldHUD()
{
}

void CWorldHUD::SetPlayerName(const TCHAR* Name)
{
	if (auto Text = NameText.lock())
	{
		Text->SetText(Name);
	}
}

void CWorldHUD::SetPlayerHP(float HP, float MaxHP)
{
	if (auto HPBar = this->HPBar.lock())
	{
		HPBar->SetRatio(HP / MaxHP);
	}
}

bool CWorldHUD::Init()
{
	if (!CWidgetContainer::Init())
	{
		return false;
	}

	auto Text = NameText.lock();

	Text->SetText(TEXT("텍스트 출력"));
	Text->SetTextColor(255, 255, 255, 255);
	Text->SetSize(80.f, 20.f);
	Text->SetFontSize(12.f);
	Text->SetAlignH(ETextAlignH::Center);
	Text->EnableShadow(true);
	Text->SetShadowOffset(1.f, 1.f);
	Text->SetShadowTextColor(128, 128, 128, 255);

	HPBar = CreateWidget<CProgressBar>("HPBar");

	auto HPBar = this->HPBar.lock();

	HPBar->SetPos(0.f, 20.f);
	HPBar->SetSize(80.f, 20.f);
	HPBar->SetTexture(EProgressBarImageType::Fill, "HPBar",
		TEXT("HPBar.png"));

	HPBar->SetTint(EProgressBarImageType::Back, 0.3f, 0.3f, 0.3f, 1.f);

	HPBar->SetBarAnchor(EProgressBarAnchor::Left);

	HPBar->SetRatio(1.f);

	return true;
}

void CWorldHUD::Update(const float DeltaTime)
{
	CWidgetContainer::Update(DeltaTime);
}

void CWorldHUD::Render()
{
	CWidgetContainer::Render();
}

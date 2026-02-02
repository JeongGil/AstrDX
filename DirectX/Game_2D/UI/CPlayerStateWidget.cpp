#include "CPlayerStateWidget.h"

#include <UI/CTextBlock.h>
#include <UI/CProgressBar.h>
#include <UI/CImage.h>

CPlayerStateWidget::CPlayerStateWidget()
{
}

CPlayerStateWidget::~CPlayerStateWidget()
{
}

void CPlayerStateWidget::SetPlayerName(const TCHAR* Name)
{
	if (auto Text = NameText.lock())
	{
		Text->SetText(Name);
	}
}

void CPlayerStateWidget::SetPlayerHP(float HP, float MaxHP)
{
	if (auto Bar = HPBar.lock())
	{
		Bar->SetRatio(HP / MaxHP);
	}
}

void CPlayerStateWidget::SetPlayerMP(float MP, float MaxMP)
{
	if (auto Bar = MPBar.lock())
	{
		Bar->SetRatio(MP / MaxMP);
	}
}

bool CPlayerStateWidget::Init()
{
	if (!CWidgetContainer::Init())
	{
		return false;
	}

	auto Back = CreateWidget<CImage>("Back", 1).lock();

	Back->SetTexture("PlayerStateBack", TEXT("CharacterHUD.png"));
	Back->SetSize(200.f, 80.f);

	NameText = CreateWidget<CTextBlock>("Text", 2);
	if (auto Text = NameText.lock())
	{
		Text->SetText(TEXT("텍스트 출력"));
		Text->SetTextColor(255, 255, 255, 255);
		Text->SetSize(200.f, 26.f);
		Text->SetFontSize(15.f);
		Text->SetAlignH(ETextAlignH::Center);
		Text->EnableShadow(true);
		Text->SetShadowOffset(2.f, 2.f);
		Text->SetShadowTextColor(128, 128, 128, 255);
	}

	HPBar = CreateWidget<CProgressBar>("HPBar");
	auto HPBar = this->HPBar.lock();

	HPBar->SetPos(24.f, 32.f);
	HPBar->SetSize(168.f, 14.f);
	HPBar->SetTexture(EProgressBarImageType::Fill, "HPBar", TEXT("HPBar.png"));

	HPBar->SetTint(EProgressBarImageType::Back,
		0.3f, 0.3f, 0.3f, 1.f);

	HPBar->SetBarAnchor(EProgressBarAnchor::Left);

	HPBar->SetRatio(1.f);

	MPBar = CreateWidget<CProgressBar>("MPBar");
	auto MPBar = this->MPBar.lock();

	MPBar->SetPos(24.f, 50.f);
	MPBar->SetSize(168.f, 14.f);
	MPBar->SetTexture(EProgressBarImageType::Fill, "MPBar", TEXT("MPBar.png"));

	MPBar->SetTint(EProgressBarImageType::Back,
		0.3f, 0.3f, 0.3f, 1.f);

	MPBar->SetBarAnchor(EProgressBarAnchor::Left);

	MPBar->SetRatio(1.f);

	return true;
}

void CPlayerStateWidget::Update(const float DeltaTime)
{
	CWidgetContainer::Update(DeltaTime);
}

void CPlayerStateWidget::Render()
{
	CWidgetContainer::Render();
}

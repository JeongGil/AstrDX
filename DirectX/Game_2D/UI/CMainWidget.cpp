#include "CMainWidget.h"

#include <UI/CButton.h>
#include <UI/CTextBlock.h>
#include <UI/CNumberWidget.h>

#include "CInventory.h"
#include "CPlayerStateWidget.h"

CMainWidget::CMainWidget()
{
}

CMainWidget::~CMainWidget()
{
}

bool CMainWidget::Init()
{
	if (!CWidgetContainer::Init())
	{
		return false;
	}

	//auto Button = CreateWidget<CButton>("TestButton").lock();

	//Button->SetPos(100.f, 50.f);
	//Button->SetSize(100.f, 50.f);
	//Button->SetTexture(EButtonState::Normal, "StartButton", TEXT("Start.png"));
	//Button->SetTint(EButtonState::Normal, FVector4(0.8f, 0.8f, 0.8f, 1.f));

	//Button->SetTexture(EButtonState::Hovered, "StartButton", TEXT("Start.png"));
	//Button->SetTint(EButtonState::Hovered, FVector4(1.f, 1.f, 1.f, 1.f));

	//Button->SetTexture(EButtonState::Click, "StartButton", TEXT("Start.png"));
	//Button->SetTint(EButtonState::Click, FVector4(0.6f, 0.6f, 0.6f, 1.f));

	//Button->SetTexture(EButtonState::Disable, "StartButton", TEXT("Start.png"));

	//Button->SetSound(EButtonEventState::Hovered, "ButtonHovered", "TeemoSmile.mp3");
	//Button->SetSound(EButtonEventState::Click, "ButtonClick", "TeemoStartClicck.mp3");

	//auto ButtonText = CreateWidget<CTextBlock>("ButtonText").lock();

	//ButtonText->SetText(TEXT("시작 버튼"));
	//ButtonText->SetTextColor(255, 255, 0, 255);

	//Button->SetChild(ButtonText);

	//auto Text = CreateWidget<CTextBlock>("Text").lock();

	//Text->SetText(TEXT("Print Text"));
	//Text->SetTextColor(255, 0, 0, 255);
	//Text->SetPos(100.f, 300.f);
	//Text->SetSize(200.f, 100.f);
	//Text->SetFontSize(30.f);
	//Text->EnableShadow(true);
	//Text->SetShadowOffset(3.f, 3.f);
	//Text->SetShadowTextColor(128, 128, 128, 255);

	PlayerStateWidget = CreateWidget<CPlayerStateWidget>("PlayerState").lock();
	if (PlayerStateWidget)
	{
		PlayerStateWidget->SetPos(50.f, 50.f);
		PlayerStateWidget->SetSize(200.f, 80.f);
	}

	if (auto Inventory = CreateWidget<CInventory>("Inventory").lock())
	{
		Inventory->SetPos(700.f, 50.f);
		Inventory->SetSize(400.f, 600.f);
		Inventory->SetZOrder(1);
	}

	if (auto Number = CreateWidget<CNumberWidget>("Number").lock())
	{
		Number->SetPos(600.f, 50.f);
		Number->SetSize(15.f, 25.f);
		Number->SetNumber(static_cast<float>(325163246));
		Number->SetNumberSize(46.f, 68.f);
		Number->SetTexture("Number", TEXT("Number.png"));
	}

	return true;
}

void CMainWidget::Update(const float DeltaTime)
{
	CWidgetContainer::Update(DeltaTime);
}

void CMainWidget::Render()
{
	CWidgetContainer::Render();
}

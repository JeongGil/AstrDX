#include "CMainWidget.h"

#include <UI/CButton.h>

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

	if (auto Button = CreateWidget<CButton>("TestButton").lock())
	{
		Button->SetPos(100.f, 50.f);
		Button->SetSize(100.f, 50.f);
		Button->SetTexture(EButtonState::Normal, "StartButton", TEXT("Start.png"));
		Button->SetTint(EButtonState::Normal, FVector4(0.8f, 0.8f, 0.8f, 1.f));

		Button->SetTexture(EButtonState::Hovered, "StartButton", TEXT("Start.png"));
		Button->SetTint(EButtonState::Hovered, FVector4(1.f, 1.f, 1.f, 1.f));

		Button->SetTexture(EButtonState::Click, "StartButton", TEXT("Start.png"));
		Button->SetTint(EButtonState::Click, FVector4(0.6f, 0.6f, 0.6f, 1.f));

		Button->SetTexture(EButtonState::Disable, "StartButton", TEXT("Start.png"));

		Button->SetSound(EButtonEventState::Hovered, "ButtonHovered", "TeemoSmile.mp3");
		Button->SetSound(EButtonEventState::Click, "ButtonClick", "TeemoStartClicck.mp3");
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

#include "CInventory.h"

#include <UI/CButton.h>
#include <UI/CImage.h>
#include <UI/CTitleBar.h>

CInventory::CInventory()
{
}

CInventory::~CInventory()
{
}

void CInventory::CloseButtonClick()
{
	SetEnable(false);
}

bool CInventory::Init()
{
	if (!CWidgetContainer::Init())
	{
		return false;
	}

	// 400, 600
	if (auto TitleBar = CreateWidget<CTitleBar>("TitleBar").lock())
	{
		TitleBar->SetSize(360.f, 40.f);
		TitleBar->SetTexture("TitleBar", TEXT("SlotBack.png"));

		TitleBar->SetText(TEXT("Inventory"));

		TitleBar->SetUpdateWidget(std::dynamic_pointer_cast<CWidget>(shared_from_this()));
	}

	if (auto CloseButton = CreateWidget<CButton>("CloseButton").lock())
	{
		CloseButton->SetPos(360.f, 0.f);
		CloseButton->SetSize(40.f, 40.f);
		CloseButton->SetTexture(EButtonState::Normal, "InvenCloseButton", TEXT("CloseButton.png"));
		CloseButton->SetTint(EButtonState::Normal, FVector4(0.8f, 0.8f, 0.8f, 1.f));

		CloseButton->SetTexture(EButtonState::Hovered, "InvenCloseButton", TEXT("CloseButton.png"));
		CloseButton->SetTint(EButtonState::Hovered, FVector4(1.f, 1.f, 1.f, 1.f));

		CloseButton->SetTexture(EButtonState::Click, "InvenCloseButton", TEXT("CloseButton.png"));
		CloseButton->SetTint(EButtonState::Click, FVector4(0.6f, 0.6f, 0.6f, 1.f));

		CloseButton->SetTexture(EButtonState::Disable, "InvenCloseButton", TEXT("CloseButton.png"));

		CloseButton->SetEventCallback<CInventory>(EButtonEventState::Click, this, &CInventory::CloseButtonClick);
	}

	if (auto Back = CreateWidget<CImage>("Back").lock())
	{
		Back->SetPos(0.f, 40.f);
		Back->SetSize(400.f, 560.f);
		Back->SetTexture("InventoryBack", TEXT("SlotBack.png"));
	}

	return true;
}

void CInventory::Update(const float DeltaTime)
{
	CWidgetContainer::Update(DeltaTime);
}

void CInventory::Render()
{
	CWidgetContainer::Render();
}

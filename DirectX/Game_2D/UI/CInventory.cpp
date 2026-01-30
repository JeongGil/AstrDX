#include "CInventory.h"

#include <UI/CButton.h>
#include <UI/CImage.h>
#include <UI/CTitleBar.h>

#include "CSlot.h"

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

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			std::string Name = "Slot" + i;

			if (auto Slot = CreateWidget<CSlot>("Name", 1).lock())
			{
				Slot->SetPos(10.f + j * 55.f, 60.f + i * 55.f);
				Slot->SetSize(50.f, 50.f);

				Slots.push_back(Slot);
			}
		}
	}

	auto Slot = Slots[0].lock();
	Slot->SetIconImage("Shield", TEXT("Icon_Shield.PNG"));
	Slot->EnableIcon(true);

	Slot = Slots[1].lock();
	Slot->SetIconImage("Sword", TEXT("Icon_Sword.PNG"));
	Slot->EnableIcon(true);

	Slot = Slots[7].lock();
	Slot->SetIconImage("Sword", TEXT("Icon_Sword.PNG"));
	Slot->EnableIcon(true);

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

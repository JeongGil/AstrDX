#include "CSlot.h"

#include <UI/CImage.h>

#include "CSlotIcon.h"

CSlot::CSlot()
{
}

CSlot::~CSlot()
{
}

void CSlot::EnableIcon(bool bEnable)
{
	Icon->SetEnable(bEnable);
}

void CSlot::SetIconImage(const std::string& Key)
{
	Icon->SetIconImage(Key);
}

void CSlot::SetIconImage(const std::string& Key, const TCHAR* FileName, const std::string& PathName)
{
	Icon->SetIconImage(Key, FileName, PathName);
}

bool CSlot::Init()
{
	if (!CWidgetContainer::Init())
	{
		return false;
	}

	if (auto Back = CreateWidget<CImage>("Back").lock())
	{
		Back->SetSize(50.f, 50.f);
		Back->SetTexture("SlotBack", TEXT("SlotBack.png"));
	}

	Icon = CreateWidget<CSlotIcon>("Icon", 1).lock();
	if (Icon)
	{
		Icon->SetPos(3.f, 3.f);
		Icon->SetSize(44.f, 44.f);

		Icon->SetEnable(false);
	}

	return true;
}

void CSlot::Update(const float DeltaTime)
{
	CWidgetContainer::Update(DeltaTime);
}

void CSlot::Render()
{
	CWidgetContainer::Render();
}

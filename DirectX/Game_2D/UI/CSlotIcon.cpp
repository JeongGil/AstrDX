#include "CSlotIcon.h"

#include <UI/CImage.h>

CSlotIcon::CSlotIcon()
{
}

CSlotIcon::CSlotIcon(const CSlotIcon& other) :
	CWidgetContainer(other)
{
	Image = FindWidget<CImage>("Back");
}

CSlotIcon::~CSlotIcon()
{
}

void CSlotIcon::SetIconImage(const std::string& Key)
{
	if (auto Image = this->Image.lock())
	{
		Image->SetTexture(Key);
	}
}

void CSlotIcon::SetIconImage(const std::string& Key, const TCHAR* FileName, const std::string& PathName)
{
	if (auto Image = this->Image.lock())
	{
		Image->SetTexture(Key, FileName, PathName);
	}
}

void CSlotIcon::MouseHovered()
{
	CWidgetContainer::MouseHovered();
}

void CSlotIcon::MouseUnHovered()
{
	CWidgetContainer::MouseUnHovered();
}

bool CSlotIcon::MouseDragStart(const FVector2& MousePos, std::shared_ptr<CWidget>& DragOperator)
{
	CWidgetContainer::MouseDragStart(MousePos, DragOperator);

	DragOperator = std::dynamic_pointer_cast<CWidget>(shared_from_this());

	return true;
}

bool CSlotIcon::MouseDrag(const FVector2& MousePos, const FVector2& MouseMove)
{
	CWidgetContainer::MouseDrag(MousePos, MouseMove);

	return true;
}

bool CSlotIcon::MouseDragEnd(const FVector2& MousePos)
{
	CWidgetContainer::MouseDragEnd(MousePos);

	return true;
}

CSlotIcon* CSlotIcon::Clone() const
{
	return new CSlotIcon(*this);
}

bool CSlotIcon::Init()
{
	if (!CWidgetContainer::Init())
	{
		return false;
	}

	Image = CreateWidget<CImage>("Back");
	if (auto Image = this->Image.lock())
	{
		Image->SetSize(44.f, 44.f);
	}

	return true;
}

void CSlotIcon::Update(const float DeltaTime)
{
	CWidgetContainer::Update(DeltaTime);
}

void CSlotIcon::Render()
{
	CWidgetContainer::Render();
}

#include "CWidgetContainer.h"

CWidgetContainer::CWidgetContainer()
{
}

CWidgetContainer::~CWidgetContainer()
{
}

bool CWidgetContainer::Init()
{
	if (!CWidget::Init())
	{
		return false;
	}

	return true;
}

void CWidgetContainer::Update(const float DeltaTime)
{
	CWidget::Update(DeltaTime);
}

void CWidgetContainer::Render()
{
	CWidget::Render();
}

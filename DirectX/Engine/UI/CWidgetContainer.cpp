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

	std::erase_if(Children, [](const std::shared_ptr<CWidget>& Widget)
		{
			if (!Widget || !Widget->GetAlive())
			{
				// TODO: OnDestroy is not implemented in the component (widget), so it is commented out
				// if (Widget) Widget->OnDestroy();
				return true;
			}
			return false;
		});

	for (const auto& Widget : Children)
	{
		if (Widget->GetEnable())
		{
			Widget->Update(DeltaTime);
		}
	}
}

void CWidgetContainer::Render()
{
	CWidget::Render();

	std::erase_if(Children, [](const std::shared_ptr<CWidget>& Widget)
		{
			if (!Widget || !Widget->GetAlive())
			{
				// TODO: OnDestroy is not implemented in the component (widget), so it is commented out
				// if (Widget) Widget->OnDestroy();
				return true;
			}
			return false;
		});

	if (Children.size() >= 2)
	{
		std::ranges::sort(Children, CWidget::SortRender);
	}

	for (const auto& Widget : Children)
	{
		if (Widget->GetEnable())
		{
			Widget->Render();
		}
	}
}

bool CWidgetContainer::CollideMouse(std::weak_ptr<CWidget>& Result, const FVector2& MousePos)
{
	std::erase_if(Children, [](const std::shared_ptr<CWidget>& Widget)
		{
			if (!Widget || !Widget->GetAlive())
			{
				// TODO: OnDestroy is not implemented in the component (widget), so it is commented out
				// if (Widget) Widget->OnDestroy();
				return true;
			}
			return false;
		});

	if (Children.size() >= 2)
	{
		std::ranges::sort(Children, CWidget::SortCollision);
	}

	for (const auto& Widget : Children)
	{
		if (!Widget->GetEnable())
		{
			continue;
		}

		if (Widget->CollideMouse(Result, MousePos))
		{
			return true;
		}
	}

	return false;
}

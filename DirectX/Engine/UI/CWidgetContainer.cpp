#include "CWidgetContainer.h"

CWidgetContainer::CWidgetContainer()
{
}

CWidgetContainer::CWidgetContainer(const CWidgetContainer& other) :
	CWidget(other)
{
	Children.reserve(other.Children.size());

	auto CloneView = other.Children | std::views::transform([](const auto& Child)
		{
			return std::shared_ptr<CWidget>(Child->Clone());
		});

	std::ranges::copy(CloneView, std::back_inserter(Children));
}

CWidgetContainer::~CWidgetContainer()
{
}

void CWidgetContainer::SetParentAll()
{
	for (const auto& Child : Children)
	{
		Child->SetParent(std::dynamic_pointer_cast<CWidgetContainer>(shared_from_this()));
		Child->SetParentAll();
	}
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

	if (CWidget::CollideMouse(Result, MousePos))
	{
		return true;
	}

	return false;
}

void CWidgetContainer::MouseHovered()
{
}

void CWidgetContainer::MouseUnHovered()
{
}

bool CWidgetContainer::MouseDragStart(const FVector2& MousePos, std::shared_ptr<CWidget>& DragOperator)
{
	return false;
}

bool CWidgetContainer::MouseDrag(const FVector2& MousePos, const FVector2& MouseMove)
{
	return false;
}

bool CWidgetContainer::MouseDragEnd(const FVector2& MousePos)
{
	return false;
}

CWidgetContainer* CWidgetContainer::Clone() const
{
	return new CWidgetContainer(*this);
}

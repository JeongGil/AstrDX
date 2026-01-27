#include "CWorldUIManager.h"

CWorldUIManager::CWorldUIManager()
{
}

CWorldUIManager::~CWorldUIManager()
{
}

bool CWorldUIManager::Init()
{
	return true;
}

void CWorldUIManager::Update(const float DeltaTime)
{
	std::erase_if(Widgets, [](const std::shared_ptr<CWidget>& Widget)
		{
			if (!Widget || !Widget->GetAlive())
			{
				// TODO: OnDestroy is not implemented in the component (widget), so it is commented out
				// if (Widget) Widget->OnDestroy();
				return true;
			}
			return false;
		});

	for (const auto& Widget : Widgets)
	{
		if (Widget->GetEnable())
		{
			Widget->Update(DeltaTime);
		}
	}
}

bool CWorldUIManager::CollideMouse(const float DeltaTime, const FVector2& MousePos)
{
	std::erase_if(Widgets, [](const std::shared_ptr<CWidget>& Widget)
		{
			if (!Widget || !Widget->GetAlive())
			{
				// TODO: OnDestroy is not implemented in the component (widget), so it is commented out
				// if (Widget) Widget->OnDestroy();
				return true;
			}
			return false;
		});

	if (Widgets.size() >= 2)
	{
		std::ranges::sort(Widgets, CWorldUIManager::SortCollision);
	}

	if (HoveredWidget.expired())
	{
		HoveredWidget.reset();
	}

	// Collision detection
	for (const auto& Widget : Widgets)
	{
		if (!Widget->GetEnable())
		{
			continue;
		}

		std::weak_ptr<CWidget> HoveredResult;
		if (Widget->CollideMouse(HoveredResult, MousePos))
		{
			auto Current = HoveredResult.lock();
			auto Prev = HoveredWidget.lock();

			if (Prev != Current)
			{
				if (Prev)
				{
					Prev->MouseUnHovered();
				}

				HoveredWidget = HoveredResult;
			}

			return true;
		}
	}

	// When nothing is colliding, reset the existing hover state
	if (auto Prev = HoveredWidget.lock())
	{
		Prev->MouseUnHovered();
		HoveredWidget.reset();
	}

	return false;
}

void CWorldUIManager::Render()
{
	std::erase_if(Widgets, [](const auto& Widget)
		{
			if (!Widget || !Widget->GetAlive())
			{
				// TODO: OnDestroy is not implemented in the component (widget), so it is commented out
				// if (Widget) Widget->OnDestroy();
				return true;
			}
			return false;
		});

	if (Widgets.size() >= 2)
	{
		std::ranges::sort(Widgets, CWorldUIManager::SortRender);
	}

	for (const auto& Widget : Widgets)
	{
		if (Widget->GetEnable())
		{
			Widget->Render();
		}
	}
}

std::weak_ptr<CWidgetContainer> CWorldUIManager::GetWidget(const std::string& Key)
{
	auto it = std::ranges::find(Widgets, Key, &CWidgetContainer::GetKey);

	if (it != Widgets.end())
	{
		return *it;
	}

	return {};
}

bool CWorldUIManager::SortRender(const std::shared_ptr<CWidgetContainer>& A, const std::shared_ptr<CWidgetContainer>& B)
{
	return A->GetZOrder() < B->GetZOrder();
}

bool CWorldUIManager::SortCollision(const std::shared_ptr<CWidgetContainer>& A,
	const std::shared_ptr<CWidgetContainer>& B)
{
	return A->GetZOrder() > B->GetZOrder();
}

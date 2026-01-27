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
				// TODO: OnDestroy는 컴포넌트(위젯)에서 미구현이므로 주석 처리
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
				// TODO: OnDestroy는 미구현이므로 주석 처리
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

	// 충돌 검사
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

	// 아무것도 충돌하지 않았을 때 기존 Hover 해제
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
				// TODO: OnDestroy는 미구현이므로 주석 처리
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

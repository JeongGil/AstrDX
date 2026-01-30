#include "CWorldUIManager.h"

#include "CWorld.h"

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

	auto World = this->World.lock();
	auto Input = World->GetInput().lock();

	auto DragWidget = this->DragWidget.lock();

	if (!DragWidget)
	{
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

						//if (Prev->GetMouseDrag())
						//{
						//	Prev->SetMouseDragEnable(false);
						//	Prev->MouseDragEnd(MousePos);
						//}
					}

					HoveredWidget = HoveredResult;
				}

				auto Hovered = HoveredWidget.lock();
				if (Input->GetMouseState(EMouseType::LButton, EInputType::Press))
				{
					std::shared_ptr<CWidget> OperatorWidget;
					if (Hovered->MouseDragStart(MousePos, OperatorWidget))
					{
						Hovered->SetMouseDragEnable(true);
						Hovered->SetZOrder(INT_MAX);
						this->DragWidget = HoveredWidget;

						if (OperatorWidget)
						{
							DragOperatorWidget.reset(OperatorWidget->Clone());
							DragOperatorWidget->SetParentAll();
							DragOperatorWidget->SetPivot(0.f, 0.f);
						}
					}
				}
				//else if (Input->GetMouseState(EMouseType::LButton, EInputType::Hold)
				//	&& Hovered->GetMouseDrag())
				//{
				//	Hovered->MouseDrag(MousePos, Input->GetMouseMove());
				//}
				//else if (Input->GetMouseState(EMouseType::LButton, EInputType::Release)
				//	&& Hovered->GetMouseDrag())
				//{
				//	Hovered->SetMouseDragEnable(false);
				//	Hovered->MouseDragEnd(MousePos);
				//	Hovered->ReplaceZOrder();
				//}

				return true;
			}
		}

		// When nothing is colliding, reset the existing hover state
		if (auto Widget = HoveredWidget.lock())
		{
			Widget->MouseUnHovered();
			HoveredWidget.reset();
		}

		return false;
	}

	if (Input->GetMouseState(EMouseType::LButton, EInputType::Hold)
		&& DragWidget->GetMouseDrag())
	{
		DragWidget->MouseDrag(MousePos, Input->GetMouseMove());
	}
	else if (Input->GetMouseState(EMouseType::LButton, EInputType::Release)
		&& DragWidget->GetMouseDrag())
	{
		DragWidget->SetMouseDragEnable(false);
		DragWidget->MouseDragEnd(MousePos);
		DragWidget->ReplaceZOrder();
		this->DragWidget.reset();
		DragOperatorWidget.reset();
	}

	return true;
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

	if (DragOperatorWidget)
	{
		auto World = this->World.lock();
		auto Input = World->GetInput().lock();

		FVector Size = DragOperatorWidget->GetSize();
		FVector Center = Size * FVector(0.5f, 0.5f, 0.f);

		DragOperatorWidget->SetPos(FVector(Input->GetMousePos()) - Center);

		DragOperatorWidget->Render();
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

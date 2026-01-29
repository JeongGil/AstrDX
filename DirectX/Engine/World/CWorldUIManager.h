#pragma once

#include "../EngineInfo.h"
#include "../UI/CWidgetContainer.h"

class CWorld;

class CWorldUIManager : public std::enable_shared_from_this<CWorldUIManager>
{
	friend CWorld;

private:
	CWorldUIManager();

public:
	~CWorldUIManager();

private:
	std::weak_ptr<CWorld> World;
	std::vector<std::shared_ptr<CWidgetContainer>> Widgets;
	std::weak_ptr<CWidget> HoveredWidget;
	std::weak_ptr<CWidget> DragWidget;

public:
	bool Init();
	void Update(const float DeltaTime);
	bool CollideMouse(const float DeltaTime, const FVector2& MousePos);
	void Render();

	std::weak_ptr<CWidgetContainer> GetWidget(const std::string& Key);

private:
	static bool SortRender(const std::shared_ptr<CWidgetContainer>& A, const std::shared_ptr<CWidgetContainer>& B);
	static bool SortCollision(const std::shared_ptr<CWidgetContainer>& A, const std::shared_ptr<CWidgetContainer>& B);

public:
	template <typename T>
	std::weak_ptr<T> CreateWidget(const std::string& Key, int ZOrder = 0)
	{
		std::shared_ptr<T> Widget;
		Widget.reset(new T);

		Widget->World = World;
		Widget->UIManager = std::dynamic_pointer_cast<CWorldUIManager>(shared_from_this());
		Widget->SetKey(Key);
		Widget->SetZOrder(ZOrder);

		if (!Widget->Init())
		{
			return std::weak_ptr<T>();
		}

		Widgets.push_back(std::dynamic_pointer_cast<CWidgetContainer>(Widget));

		return std::dynamic_pointer_cast<T>(Widget);
	}
};


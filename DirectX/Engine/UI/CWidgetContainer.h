#pragma once
#include "CWidget.h"

class CWorldUIManager;
class CWorld;

class CWidgetContainer :
    public CWidget
{
	friend CWorldUIManager;

protected:
	CWidgetContainer();

public:
	~CWidgetContainer() override;

private:
	std::vector<std::shared_ptr<CWidget>> Children;

public:
	void AddWidget(const std::shared_ptr<CWidget>& Widget)
	{
		Widget->SetParent(std::dynamic_pointer_cast<CWidget>(shared_from_this()));
		Children.push_back(Widget);
	}
public:
	bool Init() override;
	void Update(const float DeltaTime) override;
	void Render() override;
	bool CollideMouse(std::weak_ptr<CWidget>& Result, const FVector2& MousePos) override;

	template <typename T>
	std::weak_ptr<T> CreateWidget(const std::string& Key, int ZOrder = 0)
	{
		std::shared_ptr<T> Widget;

		Widget.reset(new T);

		Widget->World = World;
		Widget->UIManager = UIManager;
		Widget->SetKey(Key);
		Widget->SetParent(std::dynamic_pointer_cast<CWidget>(shared_from_this()));
		Widget->SetZOrder(ZOrder);

		if (!Widget->Init())
		{
			return std::weak_ptr<T>();
		}

		Children.push_back(std::dynamic_pointer_cast<CWidget>(Widget));

		return std::dynamic_pointer_cast<T>(Widget);
	}
};


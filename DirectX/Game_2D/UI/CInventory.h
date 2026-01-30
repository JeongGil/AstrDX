#pragma once
#include <UI\CWidgetContainer.h>

class CSlot;
class CWorldUIManager;

class CInventory :
    public CWidgetContainer
{
	friend CWorldUIManager;
	friend CWidgetContainer;

protected:
	CInventory();

public:
	~CInventory() override;

private:
	std::vector<std::weak_ptr<CSlot>> Slots;

private:
	void CloseButtonClick();

public:
	bool Init() override;
	void Update(const float DeltaTime) override;
	void Render() override;
};


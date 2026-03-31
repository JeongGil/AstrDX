#pragma once
#include <UI/CWidgetContainer.h>

class CTextBlock;
class CItemSlot;
class CWorldUIManager;

class CItemInvenWidget :
    public CWidgetContainer
{
	friend CWorldUIManager;
	friend CWidgetContainer;

protected:
	CItemInvenWidget() = default;

public:
	~CItemInvenWidget() override = default;

private:
	std::weak_ptr<CTextBlock> Title;
	std::vector<std::weak_ptr<CItemSlot>> Slots;

	float SlotSize = 0.f;
	float Pad = 0.f;

	static constexpr int COLS = 3;

public:
	bool Init() override;
	void Refresh();
};


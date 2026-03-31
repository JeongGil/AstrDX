#pragma once
#include <UI/CWidgetContainer.h>

#include "../Defines.h"

class CTextBlock;
class CItemSlot;
class CWorldUIManager;

class CWeaponInvenWidget :
	public CWidgetContainer
{
	friend CWorldUIManager;
	friend CWidgetContainer;

protected:
	CWeaponInvenWidget() = default;

public:
	~CWeaponInvenWidget() override = default;

private:
	std::weak_ptr<CTextBlock> Title;
	std::array<std::weak_ptr<CItemSlot>, INVENTORY_MAX_WEAPON> Slots;

public:
	bool Init() override;
	void Refresh();
};


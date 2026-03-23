#pragma once
#include <UI/CWidgetContainer.h>

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
};


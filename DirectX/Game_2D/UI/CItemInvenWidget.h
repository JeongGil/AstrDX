#pragma once
#include <UI/CWidgetContainer.h>

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
};


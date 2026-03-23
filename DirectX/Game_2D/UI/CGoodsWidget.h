#pragma once
#include <UI/CWidgetContainer.h>

class CWorldUIManager;

class CGoodsWidget :
    public CWidgetContainer
{
	friend CWorldUIManager;
	friend CWidgetContainer;

protected:
	CGoodsWidget() = default;

public:
	~CGoodsWidget() override = default;
};


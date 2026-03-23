#pragma once
#include <UI/CWidgetContainer.h>

class CWorldUIManager;

class CStatWidget :
    public CWidgetContainer
{
	friend CWorldUIManager;
	friend CWidgetContainer;

protected:
	CStatWidget() = default;

public:
	~CStatWidget() override = default;
};


#pragma once
#include <UI/CWidgetContainer.h>

class CWorldUIManager;

class CStageStateWidget :
    public CWidgetContainer
{
	friend CWorldUIManager;
	friend CWidgetContainer;
};


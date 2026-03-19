#pragma once
#include <UI/CWidgetContainer.h>

class CWorldUIManager;

class CTitleWidget :
    public CWidgetContainer
{
	friend CWorldUIManager;

protected:
	CTitleWidget() = default;

public:
	~CTitleWidget() override = default;

	bool Init() override;
};


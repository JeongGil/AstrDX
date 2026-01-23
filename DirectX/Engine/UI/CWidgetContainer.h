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
	std::weak_ptr<CWorld> World;

public:
	bool Init() override;
	void Update(const float DeltaTime) override;
};


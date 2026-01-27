#pragma once
#include <UI/CWidgetContainer.h>

class CWorldUIManager;

class CMainWidget :
    public CWidgetContainer
{
	friend CWorldUIManager;

protected:
	CMainWidget();

public:
	~CMainWidget() override;

public:
	bool Init() override;
	void Update(const float DeltaTime) override;
	void Render() override;
};


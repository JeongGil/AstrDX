#pragma once
#include <UI/CWidgetContainer.h>

class CWorldUIManager;

class CLoadingWidget :
    public CWidgetContainer
{
	friend  CWorldUIManager;

protected:
	CLoadingWidget();

public:
	~CLoadingWidget() override;

	bool Init() override;
	void Update(const float DeltaTime) override;
	void Render() override;
};


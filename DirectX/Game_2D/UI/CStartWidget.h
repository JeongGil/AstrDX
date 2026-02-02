#pragma once
#include <UI/CWidgetContainer.h>

class CWorldUIManager;

class CStartWidget :
    public CWidgetContainer
{
	friend CWorldUIManager;

protected:
	CStartWidget();

public:
	~CStartWidget() override;

	bool Init() override;
	void Update(const float DeltaTime) override;
	void Render() override;

private:
	void OnClickStart();
	void OnClickExit();
};


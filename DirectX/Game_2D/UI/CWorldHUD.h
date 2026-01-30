#pragma once
#include <UI/CWidgetContainer.h>

class CProgressBar;
class CTextBlock;
class CWorldUIManager;
class CWidgetComponent;

class CWorldHUD :
	public CWidgetContainer
{
	friend CWorldUIManager;
	friend CWidgetComponent;
	friend CWidgetContainer;

protected:
	CWorldHUD();

public:
	~CWorldHUD() override;

private:
	std::weak_ptr<CTextBlock> NameText;
	std::weak_ptr<CProgressBar> HPBar;

public:
	void SetPlayerName(const TCHAR* Name);
	void SetPlayerHP(float HP, float MaxHP);

	bool Init() override;
	void Update(const float DeltaTime) override;
	void Render() override;
};


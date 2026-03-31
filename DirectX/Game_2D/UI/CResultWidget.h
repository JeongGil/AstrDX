#pragma once
#include <UI/CWidgetContainer.h>

class CTextBlock;
class CImage;
class CButton;
class CItemInvenWidget;
class CWeaponInvenWidget;
class CStatWidget;
class CWorldUIManager;

class CResultWidget :
    public CWidgetContainer
{
	friend CWorldUIManager;
	friend CWidgetContainer;

protected:
	CResultWidget() = default;

public:
	~CResultWidget() override = default;

private:
	std::weak_ptr<CImage> Background;

	std::weak_ptr<CImage> InvenBackground;
	std::weak_ptr<CStatWidget> StatWidget;
	std::weak_ptr<CWeaponInvenWidget> WeaponWidget;
	std::weak_ptr<CItemInvenWidget> ItemWidget;

	std::weak_ptr<CButton> RestartButton;
	std::weak_ptr<CButton> TitleButton;

	std::weak_ptr<CTextBlock> TitleText;

public:
	bool Init() override;

private:
	void OnClickRestart();
	void OnClickTitle();
};


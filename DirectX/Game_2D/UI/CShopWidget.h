#pragma once
#include <UI/CWidgetContainer.h>

class CTextBlock;
class CWeaponInvenWidget;
class CItemInvenWidget;
class CButton;
class CStatWidget;
class CImage;
class CGoodsWidget;
class CWorldUIManager;

class CShopWidget :
    public CWidgetContainer
{
	friend CWorldUIManager;

protected:
	CShopWidget() = default;

public:
	~CShopWidget() override = default;

private:
	std::weak_ptr<CGoodsWidget> Goods;
	std::weak_ptr<CStatWidget> Stats;
	std::weak_ptr<CItemInvenWidget> Items;
	std::weak_ptr<CWeaponInvenWidget> Weapons;

	std::weak_ptr<CTextBlock> Title;

	std::weak_ptr<CButton> PlayButton;
	std::weak_ptr<CTextBlock> Playtext;

	std::weak_ptr<CImage> Background;

	int CurrentLevel;

public:
	bool Init() override;

	[[nodiscard]] int GetCurrentLevel() const
	{
		return CurrentLevel;
	}

	void SetCurrentLevel(const int NewLevel);

	[[nodiscard]] int GetNextLevel() const
	{
		return CurrentLevel + 1;
	}
};


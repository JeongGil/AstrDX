#pragma once
#include <UI/CWidgetContainer.h>

class CStatValueWidget;
class CButton;
class CTextBlock;
class CImage;
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

private:
	std::weak_ptr<CImage> Background;

	std::weak_ptr<CTextBlock> Title;

	std::weak_ptr<CButton> PrimaryButton;
	std::weak_ptr<CTextBlock> PrimaryText;

	std::weak_ptr<CButton> SecondaryButton;
	std::weak_ptr<CTextBlock> SecondaryText;

	std::vector<std::weak_ptr<CStatValueWidget>> Stats;

public:
	bool Init() override;

	void PrintPrimary();
	void PrintSecondary();
};


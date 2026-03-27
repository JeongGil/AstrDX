#pragma once
#include <UI/CWidgetContainer.h>

#include "../Defines.h"

class CButton;
class CTextBlock;
class CImage;
class CWorldUIManager;

class CStatValueWidget :
	public CWidgetContainer
{
	friend CWorldUIManager;
	friend CWidgetContainer;

protected:
	CStatValueWidget() = default;

public:
	~CStatValueWidget() override = default;

private:
	std::weak_ptr<CButton> TooltipButton;
	std::weak_ptr<CImage> StatIcon;
	std::weak_ptr<CTextBlock> StatName;
	std::weak_ptr<CTextBlock> StatValue;

	bool bIsPrimary;
	EStat::Type Type;
	int Value;

public:
	bool Init() override;

	[[nodiscard]] bool IsPrimary() const
	{
		return bIsPrimary;
	}

	void SetPrimary(const bool bIsPrimary);

	[[nodiscard]] EStat::Type GetStatType() const
	{
		return Type;
	}

	void SetStatType(const EStat::Type StatType);

	[[nodiscard]] int GetStatValue() const
	{
		return Value;
	}

	void SetStatValue(const int NewValue);

	void SetStat(EStat::Type Type, int Value, bool bIsPrimary)
	{
		SetPrimary(bIsPrimary);
		SetStatType(Type);
		SetStatValue(Value);
	}

private:
	void OnClickButton();
};


#pragma once
#include <UI/CWidgetContainer.h>

class CTextBlock;
class CImage;
class CWorldUIManager;

class CItemSlot :
	public CWidgetContainer
{
	friend CWorldUIManager;
	friend CWidgetContainer;

	inline static const FColor BASE_BACKGROUND_COLOR = FColor(37 / 255.f, 37 / 255.f, 37 / 255.f, 1.f);

protected:
	CItemSlot() = default;

public:
	~CItemSlot() override = default;

private:
	std::weak_ptr<CImage> Background;
	std::weak_ptr<CImage> SlotItem;
	std::weak_ptr<CTextBlock> ItemCount;

	int BackgroundZOrder = 0;

	float Pad = 0.f;

	TableID ItemID{ -1 };
	bool bIsWeapon = false;

	float FontSize = 0.f;

public:
	bool Init() override;
	void SetZOrder(const int ZOrder) override;

	[[nodiscard]] float GetPad() const
	{
		return Pad;
	}

	void SetPad(const float NewPad)
	{
		if (NewPad == Pad)
		{
			return;
		}

		Pad = NewPad;

		SetSize(GetSize());
	}

	[[nodiscard]] int GetBackgroundZOrder() const
	{
		return BackgroundZOrder;
	}

	void SetSize(const FVector& Size) override;
	void SetSize(float x, float y) override;

	void SetItem(TableID ID, bool bWeapon = false);

	void SetFontSize(float NewSize);

	void SetItemCount(int Count) const;

private:
	void RefreshItemCountPosAndSize() const;
};


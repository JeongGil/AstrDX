#pragma once
#include "CWidget.h"
#include "../Asset/Font/CFont.h"

enum class ETextAlignH
{
	Left,
	Center,
	Right,
};

enum class ETextAlignV
{
	Top,
	Middle,
	Bottom,
};


class CWorldUIManager;
class CWidgetContainer;

class CTextBlock :
    public CWidget
{
	friend CWorldUIManager;
	friend CWidgetContainer;
	friend CWidget;

protected:
	CTextBlock();
	CTextBlock(const CTextBlock& other);

public:
	~CTextBlock() override;

protected:
	ID2D1RenderTarget* Target{};
	std::wstring Text;
	std::weak_ptr<CFont> Font;
	IDWriteTextLayout* Layout{};
	ID2D1SolidColorBrush* TextColor{};
	bool bTransparent = false;
	float Opacity = 1.f;

	bool bShadow = false;
	ID2D1SolidColorBrush* TextShadowColor{};
	bool bShadowTransparent = false;
	float ShadowOpacity = 1.f;
	FVector2 ShadowOffset{ 1.f, 1.f };

	float FontSize = 20.f;
	ETextAlignH AlignH = ETextAlignH::Left;
	ETextAlignV AlignV = ETextAlignV::Middle;

public:
	[[nodiscard]] const std::wstring& GetText() const
	{
		return Text;
	}

	[[nodiscard]] const TCHAR* GetTextPtr() const
	{
		return Text.c_str();
	}

	[[nodiscard]] int GetTextCount() const
	{
		return static_cast<int>(Text.length());
	}

	void SetSize(const FVector& Size) override
	{
		CWidget::SetSize(Size);

		CreateTextLayout();
	}

	void SetSize(float x, float y) override
	{
		CWidget::SetSize(x, y);

		CreateTextLayout();

	}

public:
	void SetFont(const std::string& Key);
	void SetText(const TCHAR* Text);
	void SetText(int Val);
	void SetText(float Val);
	void AddText(const TCHAR* Text);
	void AddText(int Val);
	void AddText(float Val);
	void PopBack();
	void ClearText();
	void SetFontSize(float Size);
	void SetAlignH(ETextAlignH Align);
	void SetAlignV(ETextAlignV Align);

	void EnableTransparency(bool bTransparent);
	void SetOpacity(float Opacity) override;
	void AddOpacity(float Opacity);
	void SetTextColor(const FColor& Color);
	void SetTextColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

	void EnableShadow(bool bShadow);
	void EnableShadowTransparency(bool bTransparent);
	void SetShadowOpacity(float Opacity);
	void AddShadowOpacity(float Opacity);
	void SetShadowTextColor(const FColor& Color);
	void SetShadowTextColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);	
	void SetShadowOffset(const FVector2& Offset);
	void SetShadowOffset(float x, float y);

private:
	void CreateTextLayout();

public:
	bool Init() override;
	void Update(const float DeltaTime) override;
	void Render() override;
	bool CollideMouse(std::weak_ptr<CWidget>& Result, const FVector2& MousePos) override;
	void MouseHovered() override;
	void MouseUnHovered() override;
	CTextBlock* Clone() const override;
};


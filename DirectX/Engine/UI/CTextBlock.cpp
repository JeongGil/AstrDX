#include "CTextBlock.h"

#include "../CDevice.h"
#include "../Asset/CAssetManager.h"
#include "../Asset/Font/CFontManager.h"

CTextBlock::CTextBlock()
{
}

CTextBlock::~CTextBlock()
{
	SAFE_RELEASE(Layout);
}

void CTextBlock::SetFont(const std::string& Key)
{
	auto FontMgr = CAssetManager::GetInst()->GetFontManager().lock();
	Font = FontMgr->FindFont(Key);

	CreateTextLayout();
}

void CTextBlock::SetText(const TCHAR* Text)
{
	this->Text = Text;

	CreateTextLayout();
}

void CTextBlock::SetText(int Val)
{
	Text = std::to_wstring(Val);

	CreateTextLayout();
}

void CTextBlock::SetText(float Val)
{
	Text = std::to_wstring(Val);

	CreateTextLayout();
}

void CTextBlock::AddText(const TCHAR* Text)
{
	this->Text += Text;

	CreateTextLayout();
}

void CTextBlock::AddText(int Val)
{
	this->Text += std::to_wstring(Val);

	CreateTextLayout();
}

void CTextBlock::AddText(float Val)
{
	this->Text += std::to_wstring(Val);

	CreateTextLayout();
}

void CTextBlock::PopBack()
{
	Text.pop_back();

	CreateTextLayout();
}

void CTextBlock::ClearText()
{
	Text.clear();

	CreateTextLayout();
}

void CTextBlock::SetFontSize(float Size)
{
	FontSize = Size;

	if (!Layout)
	{
		CreateTextLayout();
	}
	else
	{
		DWRITE_TEXT_RANGE Range{};
		Range.startPosition = 0;
		Range.length = static_cast<UINT32>(Text.length());

		Layout->SetFontSize(FontSize, Range);
	}
}

void CTextBlock::SetAlignH(ETextAlignH Align)
{
	AlignH = Align;

	if (!Layout)
	{
		CreateTextLayout();
	}
	else
	{
		switch (AlignH)
		{
			case ETextAlignH::Left:
				Layout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
				break;
			case ETextAlignH::Center:
				Layout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
				break;
			case ETextAlignH::Right:
				Layout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
				break;
		}
	}
}

void CTextBlock::SetAlignV(ETextAlignV Align)
{
	AlignV = Align;

	if (!Layout)
	{
		CreateTextLayout();
	}
	else
	{
		switch (AlignV)
		{
			case ETextAlignV::Top:
				Layout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
				break;
			case ETextAlignV::Middle:
				Layout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
				break;
			case ETextAlignV::Bottom:
				Layout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
				break;
		}
	}
}

void CTextBlock::EnableTransparency(bool bTransparent)
{
	this->bTransparent = bTransparent;
}

void CTextBlock::SetOpacity(float Opacity)
{
	this->Opacity = std::clamp(Opacity, 0.f, 1.f);
}

void CTextBlock::AddOpacity(float Opacity)
{
	this->Opacity = std::clamp(this->Opacity + Opacity, 0.f, 1.f);
}

void CTextBlock::SetTextColor(const FColor& Color)
{
	auto FontMgr = CAssetManager::GetInst()->GetFontManager().lock();
	FontMgr->CreateFontColor(Color);

	TextColor = FontMgr->FindFontColor(Color);
}

void CTextBlock::SetTextColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	auto FontMgr = CAssetManager::GetInst()->GetFontManager().lock();
	FontMgr->CreateFontColor(r, g, b, a);

	TextColor = FontMgr->FindFontColor(r, g, b, a);
}

void CTextBlock::EnableShadow(bool bShadow)
{
	this->bShadow = bShadow;
}

void CTextBlock::EnableShadowTransparency(bool bTransparent)
{
	bShadowTransparent = bTransparent;
}

void CTextBlock::SetShadowOpacity(float Opacity)
{
	ShadowOpacity = std::clamp(Opacity, 0.f, 1.f);
}

void CTextBlock::AddShadowOpacity(float Opacity)
{
	ShadowOpacity = std::clamp(ShadowOpacity + Opacity, 0.f, 1.f);
}

void CTextBlock::SetShadowTextColor(const FColor& Color)
{
	auto FontMgr = CAssetManager::GetInst()->GetFontManager().lock();
	FontMgr->CreateFontColor(Color);

	TextShadowColor = FontMgr->FindFontColor(Color);
}

void CTextBlock::SetShadowTextColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	auto FontMgr = CAssetManager::GetInst()->GetFontManager().lock();
	FontMgr->CreateFontColor(r, g, b, a);

	TextShadowColor = FontMgr->FindFontColor(r, g, b, a);
}

void CTextBlock::SetShadowOffset(const FVector2& Offset)
{
	ShadowOffset = Offset;
}

void CTextBlock::SetShadowOffset(float x, float y)
{
	ShadowOffset.x = x;
	ShadowOffset.y = y;
}

void CTextBlock::CreateTextLayout()
{
	SAFE_RELEASE(Layout);

	auto Font = this->Font.lock();
	Layout = Font->CreateLayout(Text.c_str(), static_cast<int>(Text.length()), Size.x, Size.y);
	if (!Layout)
	{
		return;
	}

	DWRITE_TEXT_RANGE Range{};
	Range.startPosition = 0;
	Range.length = static_cast<UINT32>(Text.length());

	Layout->SetFontSize(FontSize, Range);

	switch (AlignH)
	{
		case ETextAlignH::Left:
			Layout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
			break;
		case ETextAlignH::Center:
			Layout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
			break;
		case ETextAlignH::Right:
			Layout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
			break;
	}

	switch (AlignV)
	{
		case ETextAlignV::Top:
			Layout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
			break;
		case ETextAlignV::Middle:
			Layout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
			break;
		case ETextAlignV::Bottom:
			Layout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
			break;
	}
}

bool CTextBlock::Init()
{
	if (!CWidget::Init())
	{
		return false;
	}

	Target = CDevice::GetInst()->Get2DTarget();

	SetFont("EngineDefault");
	SetText(TEXT("Text"));
	SetTextColor(FVector4::White);
	SetShadowTextColor(FVector4::Black);

	CreateTextLayout();

	return true;
}

void CTextBlock::Update(const float DeltaTime)
{
	CWidget::Update(DeltaTime);
}

void CTextBlock::Render()
{
	CWidget::Render();

	Target->BeginDraw();

	// 그림자를 출력할 경우
	if (bShadow)
	{
		D2D1_POINT_2F	ShadowPoint;

		ShadowPoint.x = RenderPos.x + ShadowOffset.x;
		ShadowPoint.y = RenderPos.y + ShadowOffset.y;

		if (bShadowTransparent)
		{
			TextShadowColor->SetOpacity(ShadowOpacity);
		}
		else
		{
			TextShadowColor->SetOpacity(1.f);
		}

		/*
		D2D1_DRAW_TEXT_OPTIONS_NONE : Default value. Does not clip even if
		it goes outside the layout rectangle.
		D2D1_DRAW_TEXT_OPTIONS_NO_SNAP : Disables pixel snapping. Recommended
		for scrolling text rendering.
		D2D1_DRAW_TEXT_OPTIONS_CLIP : Clips the text to make it visible only
		inside the layout rectangle.
		D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT : Enables font rendering
		in color.
		D2D1_DRAW_TEXT_OPTIONS_DISABLE_COLOR_BITMAP_SNAPPING : Handles
		color clipping as a bitmap.
		*/
		Target->DrawTextLayout(ShadowPoint, Layout, TextShadowColor, D2D1_DRAW_TEXT_OPTIONS_CLIP);
	}

	if (bTransparent)
	{
		TextColor->SetOpacity(Opacity);
	}
	else
	{
		TextColor->SetOpacity(1.f);
	}

	D2D1_POINT_2F Point;
	Point.x = RenderPos.x;
	Point.y = RenderPos.y;

	Target->DrawTextLayout(Point, Layout, TextColor, D2D1_DRAW_TEXT_OPTIONS_CLIP);

	Target->EndDraw();
}

bool CTextBlock::CollideMouse(std::weak_ptr<CWidget>& Result, const FVector2& MousePos)
{
	return false;
	//return CWidget::CollideMouse(Result, MousePos);
}

void CTextBlock::MouseHovered()
{
	//CWidget::MouseHovered();
}

void CTextBlock::MouseUnHovered()
{
	//CWidget::MouseUnHovered();
}

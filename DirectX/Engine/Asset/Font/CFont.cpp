#include "CFont.h"

CFont::CFont()
{
	AssetType = EAssetType::Font;
}

CFont::~CFont()
{
	SAFE_RELEASE(Format);
}

bool CFont::LoadFont(IDWriteFactory5* Factory, const TCHAR* FontName, int Weight, float FontSize,
	const TCHAR* LocalName, int Stretch)
{
	this->Factory = Factory;

	if (FAILED(Factory->CreateTextFormat(FontName, nullptr,
		(DWRITE_FONT_WEIGHT)Weight, DWRITE_FONT_STYLE_NORMAL,
		(DWRITE_FONT_STRETCH)Stretch, FontSize, LocalName,
		(IDWriteTextFormat**)&Format)))
	{
		return false;
	}

	return true;
}

IDWriteTextLayout* CFont::CreateLayout(const TCHAR* Text, int Length, float Width, float Height)
{
	IDWriteTextLayout* Layout = nullptr;

	if (FAILED(Factory->CreateTextLayout(Text, Length, Format, Width, Height, &Layout)))
	{
		return nullptr;
	}

	return Layout;
}

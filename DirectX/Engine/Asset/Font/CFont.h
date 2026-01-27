#pragma once
#include "../CAsset.h"

class CFontManager;

class CFont :
    public CAsset
{
	friend CFontManager;

protected:
	CFont();

public:
	~CFont() override;

private:
	IDWriteFactory5* Factory = nullptr;
	IDWriteTextFormat* Format = nullptr;

public:
	bool LoadFont(IDWriteFactory5* Factory, const TCHAR* FontName, int Weight, float FontSize,
		const TCHAR* LocalName, int Stretch = DWRITE_FONT_STRETCH_NORMAL);
	IDWriteTextLayout* CreateLayout(const TCHAR* Text, int Length, float Width, float Height);
};


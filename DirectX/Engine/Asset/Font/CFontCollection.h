#pragma once
#include "../CAsset.h"

class CFontManager;

class CFontCollection :
    public CAsset
{
	friend CFontManager;

protected:
	CFontCollection();

public:
	~CFontCollection() override;

private:
	IDWriteFontCollection1* Collection = nullptr;
	TCHAR FontFaceName[128] = {};

public:
	bool LoadFontCollection(IDWriteFactory5* Factory, const TCHAR* FileName, const std::string& PathName = "Font");
	
	const TCHAR* GetFontFaceName() const
	{
		return FontFaceName;
	}
};


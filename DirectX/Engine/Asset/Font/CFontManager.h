#pragma once
#include "CFont.h"
#include "CFontCollection.h"

class CAssetManager;

class CFontManager
{
	friend CAssetManager;

private:
	CFontManager();

public:
	~CFontManager();

private:
	IDWriteFactory5* WriteFactory = nullptr;

	std::unordered_map<std::string, std::shared_ptr<CFont>> Fonts;
	std::unordered_map<std::string, std::shared_ptr<CFontCollection>> FontCollections;
	std::unordered_map<unsigned int, ID2D1SolidColorBrush*>	FontColors;

public:
	bool Init();
	bool LoadFont(const std::string& Key, const TCHAR* FontName,
		int Weight, float FontSize, const TCHAR* LocalName,
		int Stretch = DWRITE_FONT_STRETCH_NORMAL);
	bool LoadFontCollection(const std::string& Key, const TCHAR* FileName, const std::string& PathName = "Font");
	const TCHAR* GetFontFaceName(const std::string& Key);
	bool CreateFontColor(const FColor& Color);
	bool CreateFontColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

	std::weak_ptr<CFont> FindFont(const std::string& Key);
	std::weak_ptr<CFontCollection> FindFontCollection(const std::string& Key);
	ID2D1SolidColorBrush* FindFontColor(const FColor& Color);
	ID2D1SolidColorBrush* FindFontColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

public:
	void ReleaseAsset(const std::string& Key);
	void ReleaseCollectionAsset(const std::string& Key);
};


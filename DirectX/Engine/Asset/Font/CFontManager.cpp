#include "CFontManager.h"

CFontManager::CFontManager()
{
}

CFontManager::~CFontManager()
{
	Fonts.clear();
	FontCollections.clear();

	for (auto& Elem : FontColors | std::views::values)
	{
		SAFE_RELEASE(Elem);
	}
	FontColors.clear();

	SAFE_RELEASE(WriteFactory);
}

bool CFontManager::Init()
{
	if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(WriteFactory), (IUnknown**)&WriteFactory)))
	{
		return false;
	}

	return true;
}

bool CFontManager::LoadFont(const std::string& Key, const TCHAR* FontName, int Weight, float FontSize,
	const TCHAR* LocalName, int Stretch)
{
	if (!FindFont(Key).expired())
	{
		return true;
	}

	std::shared_ptr<CFont> Font;
	Font.reset(new CFont);

	Font->Key = Key;

	if (!Font->LoadFont(WriteFactory, FontName, Weight, FontSize, LocalName, Stretch))
	{
		return false;
	}

	Fonts.emplace(Key, Font);

	return true;
}

bool CFontManager::LoadFontCollection(const std::string& Key, const TCHAR* FileName, const std::string& PathName)
{
	if (!FindFontCollection(Key).expired())
		return true;

	std::shared_ptr<CFontCollection> Collection;
	Collection.reset(new CFontCollection);

	Collection->Key = Key;

	if (!Collection->LoadFontCollection(WriteFactory, FileName, PathName))
	{
		return false;
	}

	FontCollections.emplace(Key, Collection);

	return true;
}

std::weak_ptr<CFont> CFontManager::FindFont(const std::string& Key)
{
	auto It = Fonts.find(Key);

	if (It == Fonts.end())
	{
		return {};
	}

	return It->second;
}

std::weak_ptr<CFontCollection> CFontManager::FindFontCollection(const std::string& Key)
{
	auto It = FontCollections.find(Key);

	if (It == FontCollections.end())
	{
		return {};
	}

	return It->second;
}

void CFontManager::ReleaseAsset(const std::string& Key)
{
	if (auto It = Fonts.find(Key); It != Fonts.end())
	{
		if (It->second.use_count() == 1)
		{
			Fonts.erase(It);
		}
	}
}

void CFontManager::ReleaseCollectionAsset(const std::string& Key)
{
	if (auto It = FontCollections.find(Key); It != FontCollections.end())
	{
		if (It->second.use_count() == 1)
		{
			FontCollections.erase(It);
		}
	}
}

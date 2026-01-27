#include "CFontCollection.h"

#include "../CPathManager.h"

CFontCollection::CFontCollection()
{
	AssetType = EAssetType::FontCollection;
}

CFontCollection::~CFontCollection()
{
	SAFE_RELEASE(Collection);
}

bool CFontCollection::LoadFontCollection(IDWriteFactory5* Factory, const TCHAR* FileName, const std::string& PathName)
{
	TCHAR FullPath[MAX_PATH] = {};

	if (const TCHAR* Path = CPathManager::FindPath(PathName))
	{
		lstrcpy(FullPath, Path);
	}

	lstrcat(FullPath, FileName);

	IDWriteFontFile* FontFile = nullptr;
	if (FAILED(Factory->CreateFontFileReference(FullPath, nullptr, &FontFile)))
	{
		return false;
	}

	IDWriteFontSetBuilder1* FontBuilder = nullptr;
	if (FAILED(Factory->CreateFontSetBuilder(&FontBuilder)))
	{
		SAFE_RELEASE(FontFile);
		return false;
	}

	FontBuilder->AddFontFile(FontFile);

	IDWriteFontSet* FontSet = nullptr;
	if (FAILED(FontBuilder->CreateFontSet(&FontSet)))
	{
		SAFE_RELEASE(FontBuilder);
		SAFE_RELEASE(FontFile);
		return false;
	}

	if (FAILED(Factory->CreateFontCollectionFromFontSet(FontSet, &Collection)))
	{
		SAFE_RELEASE(FontSet);
		SAFE_RELEASE(FontBuilder);
		SAFE_RELEASE(FontFile);
		return false;
	}

	SAFE_RELEASE(FontSet);
	SAFE_RELEASE(FontBuilder);
	SAFE_RELEASE(FontFile);

	UINT32 Count = Collection->GetFontFamilyCount();

	IDWriteFontFamily* Family = nullptr;
	if (FAILED(Collection->GetFontFamily(0, &Family)))
	{
		return false;
	}

	IDWriteLocalizedStrings* LocalizedName = nullptr;
	if (FAILED(Family->GetFamilyNames(&LocalizedName)))
	{
		return false;
	}

	if (FAILED(LocalizedName->GetString(0, FontFaceName, 128)))
	{
		SAFE_RELEASE(Family);
		return false;
	}

	SAFE_RELEASE(LocalizedName);
	SAFE_RELEASE(Family);

	return true;
}

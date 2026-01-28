#include "CFontManager.h"

#include "../../CDevice.h"

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

	LoadFontCollection("EngineDefault", TEXT("NotoSansKR-Regular.otf"), "EngineFont");

	LoadFont("EngineDefault", GetFontFaceName("EngineDefault"), 600, 20.f, TEXT("ko"));

	CreateFontColor(FColor::Black);
	CreateFontColor(FColor::White);
	CreateFontColor(FColor::Red);
	CreateFontColor(FColor::Blue);
	CreateFontColor(FColor::Green);
	CreateFontColor(255, 255, 0, 255);

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

const TCHAR* CFontManager::GetFontFaceName(const std::string& Key)
{
	auto It = FontCollections.find(Key);
	if (It == FontCollections.end())
	{
		return {};
	}

	return It->second->GetFontFaceName();
}

bool CFontManager::CreateFontColor(const FColor& Color)
{
	ID2D1SolidColorBrush* Brush = FindFontColor(Color);

	if (Brush)
	{
		return true;
	}

	uint8_t r = static_cast<uint8_t>(Color.x * 255);
	uint8_t g = static_cast<uint8_t>(Color.y * 255);
	uint8_t b = static_cast<uint8_t>(Color.z * 255);
	uint8_t a = static_cast<uint8_t>(Color.w * 255);

	uint32_t Key = a;
	Key <<= 8;

	Key |= r;
	Key <<= 8;

	Key |= g;
	Key <<= 8;

	Key |= b;

	if (FAILED(CDevice::GetInst()->Get2DTarget()->CreateSolidColorBrush(
		D2D1::ColorF(Color.x, Color.y, Color.z, Color.w), &Brush)))
	{
		return false;
	}

	FontColors.emplace(Key, Brush);

	return true;
}

bool CFontManager::CreateFontColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	ID2D1SolidColorBrush* Brush = FindFontColor(r, g, b, a);

	if (Brush)
	{
		return true;
	}

	uint32_t Key = a;
	Key <<= 8;

	Key |= r;
	Key <<= 8;

	Key |= g;
	Key <<= 8;

	Key |= b;

	if (FAILED(CDevice::GetInst()->Get2DTarget()->CreateSolidColorBrush(
		D2D1::ColorF(r / 255.f, g / 255.f, b / 255.f, a / 255.f), &Brush)))
	{
		return false;
	}

	FontColors.emplace(Key, Brush);

	return true;
}

ID2D1SolidColorBrush* CFontManager::FindFontColor(const FColor& Color)
{
	uint8_t r = static_cast<uint8_t>(Color.x * 255);
	uint8_t g = static_cast<uint8_t>(Color.y * 255);
	uint8_t b = static_cast<uint8_t>(Color.z * 255);
	uint8_t a = static_cast<uint8_t>(Color.w * 255);

	uint32_t Key = a;
	Key <<= 8;

	Key |= r;
	Key <<= 8;

	Key |= g;
	Key <<= 8;

	Key |= b;

	auto It = FontColors.find(Key);
	if (It == FontColors.end())
	{
		return {};
	}

	return It->second;
}

ID2D1SolidColorBrush* CFontManager::FindFontColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	uint32_t Key = a;
	Key <<= 8;

	Key |= r;
	Key <<= 8;

	Key |= g;
	Key <<= 8;

	Key |= b;

	auto It = FontColors.find(Key);
	if (It == FontColors.end())
	{
		return {};
	}

	return It->second;
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

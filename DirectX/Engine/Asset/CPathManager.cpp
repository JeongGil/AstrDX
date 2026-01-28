#include "CPathManager.h"

#include <ranges>

bool CPathManager::Init()
{
	TCHAR* Path = new TCHAR[MAX_PATH];
	GetModuleFileName(nullptr, Path, MAX_PATH);

	// remove exe file name.
	const int Length = lstrlen(Path);
	for (int i = Length - 1; i >= 0; i--)
	{
		if (TCHAR Char = Path[i];
			Char == '\\' || Char == '/')
		{
			memset(&Path[i + 1], 0, sizeof(TCHAR) * (Length - i - 1));
			break;
		}
	}

	Paths.emplace("Root", Path);

	CreatePath("Asset", TEXT("Asset\\"));
	CreatePath("Shader", TEXT("Shader\\"), "Asset");
	CreatePath("Texture", TEXT("Texture\\"), "Asset");
	CreatePath("Sound", TEXT("Sound\\"), "Asset");

	CreatePath("EngineAsset", TEXT("EngineAsset\\"));
	CreatePath("EngineShader", TEXT("Shader\\"), "EngineAsset");
	CreatePath("EngineTexture", TEXT("Shader\\"), "EngineAsset");
	CreatePath("EngineFont", TEXT("Font\\"), "EngineAsset");

	return true;
}

void CPathManager::Clear()
{
	for (TCHAR* Path : Paths | std::views::values)
	{
		delete[] Path;
	}

	Paths.clear();
}

bool CPathManager::CreatePath(const std::string& Key, const TCHAR* Path, const std::string& BasePathName)
{
	if (Paths.contains(Key))
	{
		return false;
	}

	const TCHAR* const BasePath = FindPath(BasePathName);
	if (!BasePath)
	{
		return false;
	}

	TCHAR* FullPath = new TCHAR[MAX_PATH];
	memset(FullPath, 0, sizeof(TCHAR) * MAX_PATH);

	lstrcpy(FullPath, BasePath);
	lstrcat(FullPath, Path);

	Paths.emplace(Key, FullPath);

	return true;
}

const TCHAR* CPathManager::FindPath(const std::string& Key)
{
	const auto it = Paths.find(Key);
	if (it == Paths.end())
	{
		return nullptr;
	}
	else
	{
		return it->second;
	}
}

const char* CPathManager::FindPathMultibyte(const std::string Key)
{
	auto It = Paths.find(Key);

	if (It == Paths.end())
	{
		return nullptr;
	}

	static char	Path[MAX_PATH] = {};

	memset(Path, 0, MAX_PATH);

	int Length = WideCharToMultiByte(CP_ACP, 0, It->second, -1,
		nullptr, 0, nullptr, nullptr);
	WideCharToMultiByte(CP_ACP, 0, It->second, -1, Path, Length,
		nullptr, nullptr);

	return Path;
}

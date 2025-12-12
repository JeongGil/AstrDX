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

bool CPathManager::CreatePath(const std::string& Name, const TCHAR* Path, const std::string& BasePathName)
{
	if (Paths.contains(Name))
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

	Paths.emplace(Name, FullPath);

	return true;
}

const TCHAR* CPathManager::FindPath(const std::string& Key)
{
	const auto it = Paths.find(Key);
	return it == Paths.end() ? nullptr : it->second;
}

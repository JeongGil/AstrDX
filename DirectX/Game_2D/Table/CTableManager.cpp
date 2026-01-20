#include "CTableManager.h"

#include <Asset/CPathManager.h>

#include "CharacterVisualTable.h"
#include "ItemTable.h"
#include "MiscTable.h"
#include "WeaponTable.h"
#include "../Strings.h"
#include "../Utility.h"

bool CTableManager::Init()
{
	CPathManager::CreatePath(Key::Path::Table, TEXT("tables\\"), Key::Path::Brotato);

	return true;
}

void CTableManager::LoadTables()
{
	//ItemTable::GetInst().Load(GetFilePath(TEXT("ItemInfo")));
	MiscTable::GetInst().Load(GetFilePath(TEXT("Misc")));
	CharacterVisualTable::GetInst().Load(GetFilePath(TEXT("CharacterVisual")));
	WeaponTable::GetInst().Load(GetFilePath(TEXT("Weapon")));
}

std::string CTableManager::GetFilePath(const TCHAR* FileName, const std::string& PathKey)
{
	const std::string EXT = ".CSV";

	std::string Path;

	const auto BasePath = CPathManager::FindPath(PathKey);
	if (BasePath != nullptr)
	{
		Path += TCharToStdString(BasePath);
	}

	Path += TCharToStdString(FileName);
	Path += EXT;

	return Path;
}

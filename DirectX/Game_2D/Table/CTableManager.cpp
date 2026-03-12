#include "CTableManager.h"

#include <Asset/CPathManager.h>

#include "CharacterBaseTable.h"
#include "CharacterVisualTable.h"
#include "EnemyTable.h"
#include "ItemTable.h"
#include "MiscTable.h"
#include "WeaponSetBonusTable.h"
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
	MiscTable::GetInst().Load(GetFilePath(TEXT("Misc")));
	CharacterVisualTable::GetInst().Load(GetFilePath(TEXT("CharacterVisual")));
	WeaponTable::GetInst().Load(GetFilePath(TEXT("Weapon")));
	WeaponSetBonusTable::GetInst().Load(GetFilePath(TEXT("WeaponSetBonus")));
	EnemyTable::GetInst().Load(GetFilePath(TEXT("Enemy")));
	//CharacterBaseTable::GetInst().Load(GetFilePath(TEXT("CharacterBase")));
	//ItemTable::GetInst().Load(GetFilePath(TEXT("Item")));
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

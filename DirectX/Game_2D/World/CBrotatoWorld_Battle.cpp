#include "CBrotatoWorld_Battle.h"

#include <Asset/CPathManager.h>

#include "../Strings.h"
#include "../Character/CPlayerCharacter.h"
#include "../Table/CTableManager.h"

bool CBrotatoWorld_Battle::Init()
{
	if (!CWorld::Init())
	{
		return false;
	}

	CPathManager::CreatePath(Key::Path::Brotato, TEXT("Brotato\\"), Key::Path::Asset);

	CTableManager::GetInst().Init();
	CTableManager::GetInst().LoadTables();

	auto WPC = CreateGameObject<CPlayerCharacter>(Key::Obj::PC);
	if (auto PC = WPC.lock())
	{
		PC->SetWorldPosition(0, 0);
	}

	//auto WBG = 

	return true;
}

void CBrotatoWorld_Battle::Update(const float DeltaTime)
{
	CWorld::Update(DeltaTime);

	RemainStageTime -= DeltaTime;

	if (RemainStageTime <= 0.f)
	{
		FinishStage(true);
	}
}

void CBrotatoWorld_Battle::FinishStage(bool bClear)
{
}

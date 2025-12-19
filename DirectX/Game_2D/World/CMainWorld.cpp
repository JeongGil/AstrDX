#include "CMainWorld.h"

#include "../Monster/CMonster.h"
#include "../Monster/CMonsterSpawnPoint.h"
#include "../Player/CPlayer.h"

bool CMainWorld::Init()
{
	CWorld::Init();

	auto WeakPlayer = CreateGameObject<CPlayer>("Player");

	auto WeakMonster = CreateGameObject<CMonster>("Monster");
	if (auto Monster = WeakMonster.lock())
	{
		Monster->SetWorldPosition(-400, 300);
		Monster->SetWorldRotationZ(180.f);
	}

	WeakMonster = CreateGameObject<CMonster>("Monster");
	if (auto Monster = WeakMonster.lock())
	{
		Monster->SetWorldPosition(400, 300);
		Monster->SetWorldRotationZ(180.f);
	}

	auto WeakMSP = CreateGameObject<CMonsterSpawnPoint>("SpawnPoint");
	if (auto MSP = WeakMSP.lock())
	{
		MSP->SetWorldPosition(-400.f, -300.f);
		MSP->SetWorldRotationZ(20.f);
		MSP->SetSpawnType<CMonster>();
		MSP->SetSpawnTime(5.f);
	}

	return true;
}

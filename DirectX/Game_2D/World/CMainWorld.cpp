#include "CMainWorld.h"

#include "../Monster/CMonster.h"
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
		Monster->SetWorldScale(0.5f, 0.5f, 0.5f);
	}

	WeakMonster = CreateGameObject<CMonster>("Monster");
	if (auto Monster = WeakMonster.lock())
	{
		Monster->SetWorldPosition(400, 300);
		Monster->SetWorldRotationZ(180.f);
		Monster->SetWorldScale(0.5f, 0.5f, 0.5f);
	}

	return true;
}

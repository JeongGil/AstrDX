#include "CMainWorld.h"

#include "../Player/CPlayer.h"

bool CMainWorld::Init()
{
	auto WeakPlayer = CreateGameObject<CPlayer>("Player");

	return true;
}

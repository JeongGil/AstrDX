#include <CEngine.h>

#include "CGlobalSetting.h"
#include "Character/CEnemy.h"
#include "Character/CPlayerCharacter.h"
#include "Character/CProjectile.h"
#include "Character/CWeapon_Battle.h"
#include "Component/CStateComponent.h"
#include "Monster/CMonster.h"
#include "Monster/CMonsterSpawnPoint.h"
#include "Player/CBullet.h"
#include "Player/CMissile.h"
#include "Player/CPlayer.h"
#include "World/CBrotatoWorld_Battle.h"
#include "World/CBrotatoWorld_Title.h"
#include "World/CMainWorld.h"
#include "World/CStartWorld.h"
#include "World/CWorldManager.h"

#ifdef _DEBUG
// link debug lib
#pragma comment(lib, "Engine_Debug.lib")

#else
#pragma comment(lib, "Engine.lib")

#endif


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	CEngine::GetInst()->CreateEngineSetting<CGlobalSetting>();

	if (!CEngine::GetInst()->Init(hInstance, TEXT("GameClient"),
		IDI_ICON1, IDI_ICON1,
		1280, 720, true))
	{
		CEngine::DestroyInst();

		return 0;
	}

	CEngine::CreateCDO<CPlayer>();
	CEngine::CreateCDO<CBullet>();
	CEngine::CreateCDO<CMissile>();
	CEngine::CreateCDO<CMonster>();
	CEngine::CreateCDO<CMonsterSpawnPoint>();
	CEngine::CreateCDO<CStateComponent>();

	CEngine::CreateCDO<CPlayerCharacter>();
	CEngine::CreateCDO<CEnemy>();
	CEngine::CreateCDO<CWeapon_Battle>();
	CEngine::CreateCDO<CProjectile>();

	//CWorldManager::GetInst()->CreateWorld<CStartWorld>(false);
	//CWorldManager::GetInst()->CreateWorld<CMainWorld>(false);
	//CWorldManager::GetInst()->CreateWorld<CBrotatoWorld_Battle>(false);
	CWorldManager::GetInst()->CreateWorld<CBrotatoWorld_Title>(false);

	int ret = CEngine::GetInst()->Run();

	CEngine::DestroyInst();

	return ret;
}

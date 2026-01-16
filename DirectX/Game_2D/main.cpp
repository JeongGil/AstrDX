#include <CEngine.h>

#include "Monster/CMonster.h"
#include "Monster/CMonsterSpawnPoint.h"
#include "Player/CBullet.h"
#include "Player/CMissile.h"
#include "Player/CPlayer.h"
#include "World/CMainWorld.h"
#include "World/CWorldManager.h"
#include "Component/CStateComponent.h"
#include "CGlobalSetting.h"
#include "World/CBrotatoWorld_Battle.h"

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

	//CWorldManager::GetInst()->CreateWorld<CMainWorld>(false);
	CWorldManager::GetInst()->CreateWorld<CBrotatoWorld_Battle>(false);

	int ret = CEngine::GetInst()->Run();

	CEngine::DestroyInst();

	return ret;
}

#include "CMainWorld.h"

#include <Asset/CAssetManager.h>
#include <Asset/Animation2D/CAnimation2DManager.h>
#include "../Monster/CMonster.h"
#include "../Monster/CMonsterSpawnPoint.h"
#include "../Player/CPlayer.h"

bool CMainWorld::Init()
{
	if (!CWorld::Init())
	{
		return false;
	}

	LoadAnimation2D();

	auto WeakPlayer = CreateGameObject<CPlayer>("Player");

	auto WeakMonster = CreateGameObject<CMonster>("Monster");
	if (auto Monster = WeakMonster.lock())
	{
		Monster->SetWorldPosition(-400, 300);
		//Monster->SetWorldRotationZ(180.f);
	}

	WeakMonster = CreateGameObject<CMonster>("Monster");
	if (auto Monster = WeakMonster.lock())
	{
		Monster->SetWorldPosition(400, 300);
		//Monster->SetWorldRotationZ(180.f);
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

void CMainWorld::LoadAnimation2D()
{
	if (auto AnimMgr = CAssetManager::GetInst()->GetAnimation2DManager().lock())
	{
		AnimMgr->CreateAnimation("PlayerIdle");
		AnimMgr->SetAnimation2DTextureType("PlayerIdle", EAnimation2DTextureType::Frame);

		std::vector<const TCHAR*> TexFileNames;

		for (int i = 0; i < 7; i++)
		{
			auto FileName = new TCHAR[MAX_PATH];
			wsprintf(FileName, TEXT("Player/PlayerFrame/adventurer-get-up-0%d.png"), i);
			TexFileNames.push_back(FileName);
		}

		AnimMgr->SetTextures("PlayerIdle", "PlayerIdle", TexFileNames);

		for (auto& FileName : TexFileNames)
		{
			delete[] FileName;
		}

		TexFileNames.clear();

		AnimMgr->AddFrame("PlayerIdle", 7, 0.f, 0.f, 50.f, 37.f);

		AnimMgr->CreateAnimation("PlayerWalk");
		AnimMgr->SetAnimation2DTextureType("PlayerWalk", EAnimation2DTextureType::SpriteSheet);

		AnimMgr->SetTexture("PlayerWalk", "PlayerSheet", TEXT("Player/Player.png"));

		AnimMgr->AddFrame("PlayerWalk", 200.f, 222.f, 50.f, 37.f);
		AnimMgr->AddFrame("PlayerWalk", 250.f, 222.f, 50.f, 37.f);
		AnimMgr->AddFrame("PlayerWalk", 300.f, 222.f, 50.f, 37.f);
		AnimMgr->AddFrame("PlayerWalk", 0.f, 259.f, 50.f, 37.f);
		AnimMgr->AddFrame("PlayerWalk", 50.f, 259.f, 50.f, 37.f);
		AnimMgr->AddFrame("PlayerWalk", 100.f, 259.f, 50.f, 37.f);

		AnimMgr->CreateAnimation("PlayerAttack");
		AnimMgr->SetAnimation2DTextureType("PlayerAttack", EAnimation2DTextureType::SpriteSheet);

		AnimMgr->SetTexture("PlayerAttack", "PlayerSheet", TEXT("Player/Player.png"));

		AnimMgr->AddFrame("PlayerAttack", 0.f, 0.f, 50.f, 37.f);
		AnimMgr->AddFrame("PlayerAttack", 50.f, 0.f, 50.f, 37.f);
		AnimMgr->AddFrame("PlayerAttack", 100.f, 0.f, 50.f, 37.f);
		AnimMgr->AddFrame("PlayerAttack", 150.f, 0.f, 50.f, 37.f);

		//======= Monster =======
		constexpr float w = 45.f;
		constexpr float h = 60.f;

		AnimMgr->CreateAnimation("MonsterIdle");
		AnimMgr->SetAnimation2DTextureType("MonsterIdle", EAnimation2DTextureType::SpriteSheet);

		AnimMgr->SetTexture("MonsterIdle", "MonsterSheet", TEXT("Monster.png"));

		for (int i = 0; i < 14; i++)
		{
			AnimMgr->AddFrame("MonsterIdle", 0 + w * i, h * 1, w, h);
		}

		AnimMgr->CreateAnimation("MonsterAttack");
		AnimMgr->SetAnimation2DTextureType("MonsterAttack", EAnimation2DTextureType::SpriteSheet);

		AnimMgr->SetTexture("MonsterAttack", "MonsterSheet", TEXT("Monster.png"));

		for (int i = 0; i < 5; i++)
		{
			AnimMgr->AddFrame("MonsterAttack", 0 + w * i, h * 2, w, h);
		}

		//======= Explosion =======
		AnimMgr->CreateAnimation("Explosion");
		AnimMgr->SetAnimation2DTextureType("Explosion", EAnimation2DTextureType::Frame);

		for (int i = 1; i <= 89; i++)
		{
			auto FileName = new TCHAR[MAX_PATH];
			memset(FileName, 0, sizeof(TCHAR) * MAX_PATH);

			wsprintf(FileName, TEXT("Explosion/Explosion%d.png"), i);

			TexFileNames.push_back(FileName);
		}

		AnimMgr->SetTextures("Explosion", "Explosion", TexFileNames);

		for (auto& elem : TexFileNames)
		{
			delete[] elem;
		}
		TexFileNames.clear();

		AnimMgr->AddFrame("Explosion", 89, 0, 0, 320, 240);
	}
}

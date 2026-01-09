#include "CMainWorld.h"

#include <Asset/CAssetManager.h>
#include <Asset/Animation2D/CAnimation2DManager.h>
#include <Asset/CPathManager.h>

#include "../Monster/CMonster.h"
#include "../Monster/CMonsterSpawnPoint.h"
#include "../Player/CPlayer.h"
#include "../Strings.h"

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
	WorldAssetManager->CreateAnimation("PlayerIdle");
	WorldAssetManager->SetAnimation2DTextureType("PlayerIdle", EAnimation2DTextureType::Frame);

	std::vector<const TCHAR*> TexFileNames;

	for (int i = 0; i < 7; i++)
	{
		auto FileName = new TCHAR[MAX_PATH];
		wsprintf(FileName, TEXT("Player/PlayerFrame/adventurer-get-up-0%d.png"), i);
		TexFileNames.push_back(FileName);
	}

	WorldAssetManager->SetTextures("PlayerIdle", "PlayerIdle", TexFileNames);

	for (auto& FileName : TexFileNames)
	{
		delete[] FileName;
	}

	TexFileNames.clear();

	WorldAssetManager->AddFrame("PlayerIdle", 7, 0.f, 0.f, 50.f, 37.f);

	WorldAssetManager->CreateAnimation("PlayerWalk");
	WorldAssetManager->SetAnimation2DTextureType("PlayerWalk", EAnimation2DTextureType::SpriteSheet);

	WorldAssetManager->SetTexture("PlayerWalk", "PlayerSheet", TEXT("Player/Player.png"));

	WorldAssetManager->AddFrame("PlayerWalk", 200.f, 222.f, 50.f, 37.f);
	WorldAssetManager->AddFrame("PlayerWalk", 250.f, 222.f, 50.f, 37.f);
	WorldAssetManager->AddFrame("PlayerWalk", 300.f, 222.f, 50.f, 37.f);
	WorldAssetManager->AddFrame("PlayerWalk", 0.f, 259.f, 50.f, 37.f);
	WorldAssetManager->AddFrame("PlayerWalk", 50.f, 259.f, 50.f, 37.f);
	WorldAssetManager->AddFrame("PlayerWalk", 100.f, 259.f, 50.f, 37.f);

	WorldAssetManager->CreateAnimation("PlayerAttack");
	WorldAssetManager->SetAnimation2DTextureType("PlayerAttack", EAnimation2DTextureType::SpriteSheet);

	WorldAssetManager->SetTexture("PlayerAttack", "PlayerSheet", TEXT("Player/Player.png"));

	WorldAssetManager->AddFrame("PlayerAttack", 0.f, 0.f, 50.f, 37.f);
	WorldAssetManager->AddFrame("PlayerAttack", 50.f, 0.f, 50.f, 37.f);
	WorldAssetManager->AddFrame("PlayerAttack", 100.f, 0.f, 50.f, 37.f);
	WorldAssetManager->AddFrame("PlayerAttack", 150.f, 0.f, 50.f, 37.f);

	//======= Monster =======
	constexpr float w = 45.f;
	constexpr float h = 60.f;

	WorldAssetManager->CreateAnimation("MonsterIdle");
	WorldAssetManager->SetAnimation2DTextureType("MonsterIdle", EAnimation2DTextureType::SpriteSheet);

	WorldAssetManager->SetTexture("MonsterIdle", "MonsterSheet", TEXT("Monster.png"));

	for (int i = 0; i < 14; i++)
	{
		WorldAssetManager->AddFrame("MonsterIdle", 0 + w * i, h * 1, w, h);
	}

	WorldAssetManager->CreateAnimation("MonsterAttack");
	WorldAssetManager->SetAnimation2DTextureType("MonsterAttack", EAnimation2DTextureType::SpriteSheet);

	WorldAssetManager->SetTexture("MonsterAttack", "MonsterSheet", TEXT("Monster.png"));

	for (int i = 0; i < 5; i++)
	{
		WorldAssetManager->AddFrame("MonsterAttack", 0 + w * i, h * 2, w, h);
	}

	//======= Explosion =======
	WorldAssetManager->CreateAnimation("Explosion");
	WorldAssetManager->SetAnimation2DTextureType("Explosion", EAnimation2DTextureType::Frame);

	for (int i = 1; i <= 89; i++)
	{
		auto FileName = new TCHAR[MAX_PATH];
		memset(FileName, 0, sizeof(TCHAR) * MAX_PATH);

		wsprintf(FileName, TEXT("Explosion/Explosion%d.png"), i);

		TexFileNames.push_back(FileName);
	}

	WorldAssetManager->SetTextures("Explosion", "Explosion", TexFileNames);

	for (auto& elem : TexFileNames)
	{
		delete[] elem;
	}
	TexFileNames.clear();

	WorldAssetManager->AddFrame("Explosion", 89, 0, 0, 320, 240);

#pragma region Brotato
	CPathManager::CreatePath(Key::Path::Brotato, TEXT("Brotato\\"), Path::Asset);

	//======= Character =======
	WorldAssetManager->LoadTexture(Key::Tex::Legs, TEXT("entities/units/player/legs.png"), Path::Brotato);
	WorldAssetManager->LoadTexture(Key::Tex::Potato, TEXT("entities/units/player/potato.png"), Path::Brotato);

	WorldAssetManager->LoadTexture(Key::Tex::EngineerEyes, TEXT("items/characters/engineer/engineer_eyes.png"), Path::Brotato);
	WorldAssetManager->LoadTexture(Key::Tex::EngineerMouth, TEXT("items/characters/engineer/engineer_mouth.png"), Path::Brotato);
	WorldAssetManager->LoadTexture(Key::Tex::EngineerIcon, TEXT("items/characters/engineer/engineer_icon.png"), Path::Brotato);

	WorldAssetManager->LoadTexture(Key::Tex::RangerEyes, TEXT("items/characters/ranger/ranger_eyes.png"), Path::Brotato);
	WorldAssetManager->LoadTexture(Key::Tex::RangerMouth, TEXT("items/characters/ranger/ranger_mouth.png"), Path::Brotato);
	WorldAssetManager->LoadTexture(Key::Tex::RangerIcon, TEXT("items/characters/ranger/ranger_icon.png"), Path::Brotato);

	//======= NPCs =======
	WorldAssetManager->LoadTexture(Key::Tex::BabyAlien, TEXT("entities/units/enemies/baby_alien/baby_alien.png"), Path::Brotato);
	WorldAssetManager->LoadTexture(Key::Tex::BabyAlienIcon, TEXT("entities/units/enemies/baby_alien/baby_alien_icon.png"), Path::Brotato);

	WorldAssetManager->LoadTexture(Key::Tex::Spitter, TEXT("entities/units/enemies/Spitter/spitter.png"), Path::Brotato);
	WorldAssetManager->LoadTexture(Key::Tex::SpitterIcon, TEXT("entities/units/enemies/Spitter/SpitterIcon.png"), Path::Brotato);

	//======= HitEffect =======
	WorldAssetManager->CreateAnimation(Key::Anim::HitEffect);
	WorldAssetManager->SetAnimation2DTextureType(Key::Anim::HitEffect, EAnimation2DTextureType::Frame);

	for (int i = 0; i < 3; i++)
	{
		auto FileName = new TCHAR[MAX_PATH];
		memset(FileName, 0, sizeof(TCHAR) * MAX_PATH);

		wsprintf(FileName, TEXT("visual_effects/hit_effect/sprites/frame000%d.png"), i);

		TexFileNames.push_back(FileName);
	}

	WorldAssetManager->SetTextures(Key::Anim::HitEffect, Key::Tex::HitEffect, TexFileNames, Path::Brotato);

	for (auto& elem : TexFileNames)
	{
		delete[] elem;
	}
	TexFileNames.clear();

	//====== Weapons ======
	WorldAssetManager->LoadTexture(Key::Tex::Wrench, TEXT("weapons/melee/wrench/wrench.png"), Path::Brotato);
	WorldAssetManager->LoadTexture(Key::Tex::WrenchIcon, TEXT("weapons/melee/wrench/wrench_icon.png"), Path::Brotato);

	WorldAssetManager->LoadTexture(Key::Tex::Fist, TEXT("weapons/melee/fist/fist_short.png"), Path::Brotato);
	WorldAssetManager->LoadTexture(Key::Tex::FistIcon, TEXT("weapons/melee/wrench/fist_icon.png"), Path::Brotato);

	WorldAssetManager->LoadTexture(Key::Tex::Crossbow, TEXT("weapons/ranged/crossbow/crossbow.png"), Path::Brotato);
	WorldAssetManager->LoadTexture(Key::Tex::CrossbowEmpty, TEXT("weapons/ranged/crossbow/crossbow_empty.png"), Path::Brotato);
	WorldAssetManager->LoadTexture(Key::Tex::CrossbowProjectile, TEXT("weapons/ranged/crossbow/crossbow_projectile.png"), Path::Brotato);
	WorldAssetManager->LoadTexture(Key::Tex::CrossbowIcon, TEXT("weapons/ranged/crossbow/crossbow_icon.png"), Path::Brotato);

	//======= Etc =======
	WorldAssetManager->LoadTexture(Key::Tex::BulletEnemy, TEXT("projectiles/bullet_enemy/bullet_enemy.png"), Path::Brotato);

	WorldAssetManager->LoadTexture(Key::Tex::EntityBirth, TEXT("entities/birth/entity_birth.png"), Path::Brotato);
	WorldAssetManager->LoadTexture(Key::Tex::StructureBirth, TEXT("entities/birth/structure_birth.png"), Path::Brotato);
#pragma endregion Brotato
}

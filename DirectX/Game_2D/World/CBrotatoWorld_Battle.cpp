#include "CBrotatoWorld_Battle.h"

#include <array>
#include <algorithm>
#include <cmath>
#include <numbers>
#include <CEngine.h>

#include <Asset/CPathManager.h>
#include <Component/CMeshComponent.h>
#include <Render/CRenderManager.h>
#include <Component/CColliderBox2D.h>

#include "../Map/CBrotatoTile.h"
#include "../Strings.h"
#include "../Character/CCameraObject.h"
#include "../Character/CEnemy.h"
#include "../Character/CPlayerCharacter.h"
#include "../Inventory/CInventoryData.h"
#include "../Table/CharacterBaseTable.h"
#include "../Table/CharacterVisualTable.h"
#include "../Table/CTableManager.h"
#include "../Table/EnemyTable.h"
#include "../Table/MiscTable.h"
#include "../Table/ProjectileTable.h"
#include "../Table/WeaponTable.h"

bool CBrotatoWorld_Battle::Init()
{
	if (!CWorld::Init())
	{
		return false;
	}

	CRenderManager::GetInst()->CreateLayer("CharacterLeg", ERenderOrder::CharacterLeg, ERenderListSort::Y);
	CRenderManager::GetInst()->CreateLayer("CharacterBody", ERenderOrder::CharacterBody, ERenderListSort::Y);
	CRenderManager::GetInst()->CreateLayer("CharacterDeco", ERenderOrder::CharacterDeco, ERenderListSort::Y);
	CRenderManager::GetInst()->CreateLayer("CharacterItem", ERenderOrder::CharacterItem, ERenderListSort::Y);
	CRenderManager::GetInst()->CreateLayer("CharacterWeapon", ERenderOrder::CharacterWeapon, ERenderListSort::Y);
	CRenderManager::GetInst()->CreateLayer("Enemy", ERenderOrder::Enemy, ERenderListSort::Y);

	CPathManager::CreatePath(Key::Path::Brotato, TEXT("Brotato\\"), Key::Path::Asset);

	CTableManager::GetInst().Init();
	CTableManager::GetInst().LoadTables();

	LoadAnimation2D();

	//CInventoryData::GetInst().AddWeapon(TableID(1));
	//CInventoryData::GetInst().AddWeapon(TableID(1));
	//CInventoryData::GetInst().AddWeapon(TableID(1));

	auto WPC = CreateGameObject<CPlayerCharacter>(Key::Obj::PC);
	if (auto PC = WPC.lock())
	{
		PC->SetWorldPosition(0, 0);
		PC->SetCharacterVisual(TableID(1));

		for (size_t i = 0; i < CInventoryData::GetInst().GetWeaponCount(); i++)
		{
			auto Weapon = CInventoryData::GetInst().GetWeapon(i);
			PC->AddWeapon(Weapon);
		}
	}

	//static int Counter = 0;
	//auto WNPC = CreateGameObject<CEnemy>("Monster_" + std::to_string(Counter));
	//if (auto NPC = WNPC.lock())
	//{
	//	NPC->SetWorldPosition(300, 300);
	//	NPC->SetEnemyInfoID(TableID(1));
	//	//NPC->SetEnemyInfoID(TableID(2));
	//}

	EnemyTableIDs.emplace_back(1);

	SubCameraObj = CreateGameObject<CCameraObject>("SubCam");

	if (auto ColObj = CreateGameObject<CGameObject>("EdgeCollider").lock())
	{
		for (int i = 0; i < EdgeColliders.size(); i++)
		{
			EdgeColliders[i] = ColObj->CreateComponent<CColliderBox2D>("Collider_" + std::to_string(i));
			if (auto Collider = EdgeColliders[i].lock())
			{
				Collider->SetCollisionProfile("MapBoundary");
				Collider->SetStatic(true);

#if defined(_DEBUG) || defined(DEBUG)
				Collider->SetDrawDebug(true);
#endif
			}
		}
	}

	auto Misc = MiscTable::GetInst().Get();
	SetTileCount(Misc->MapSizeX, Misc->MapSizeY);
	CreateTileMap();

	return true;
}

void CBrotatoWorld_Battle::Update(const float DeltaTime)
{
	CWorld::Update(DeltaTime);

	RemainStageTime -= DeltaTime;

	if (RemainStageTime <= 0.f)
	{
		FinishStage(true);
		return;
	}

	UpdateEnemySpawn(DeltaTime);
}

void CBrotatoWorld_Battle::FinishStage(bool bClear)
{
}

void CBrotatoWorld_Battle::UpdateEnemySpawn(float DeltaTime)
{
	if (EnemyTableIDs.empty() || EnemySpawnIntervalSec <= 0.f)
	{
		return;
	}

	auto Player = FindObjectOfType<CPlayerCharacter>().lock();
	if (!Player)
	{
		return;
	}

	ElapsedEnemySpawnTime += DeltaTime;
	while (ElapsedEnemySpawnTime >= EnemySpawnIntervalSec)
	{
		ElapsedEnemySpawnTime -= EnemySpawnIntervalSec;

		FVector SpawnPos;
		if (!TryGetEnemySpawnPosition(Player->GetWorldPosition(), EnemySpawnRadius, SpawnPos))
		{
			continue;
		}

		auto Enemy = CreateGameObject<CEnemy>("Monster_" + std::to_string(SpawnedEnemyCount++)).lock();
		if (!Enemy)
		{
			continue;
		}

		std::uniform_int_distribution<size_t> EnemyIDDist(0, EnemyTableIDs.size() - 1);
		const TableID EnemyID = EnemyTableIDs[EnemyIDDist(CEngine::GetInst()->GetMT())];

		Enemy->SetWorldPosition(SpawnPos);
		Enemy->SetEnemyInfoID(EnemyID);
	}
}

bool CBrotatoWorld_Battle::TryGetEnemySpawnPosition(const FVector& Origin, float Radius, FVector& OutSpawnPos) const
{
	if (TileCountX <= 0 || TileCountY <= 0 || Radius <= 0.f)
	{
		return false;
	}

	constexpr float TileSize = 64.f;
	const float MinX = -TileCountX * TileSize * 0.5f + TileSize * 0.5f;
	const float MaxX = MinX + TileSize * (TileCountX - 1);
	const float MinY = -TileCountY * TileSize * 0.5f + TileSize * 0.5f;
	const float MaxY = MinY + TileSize * (TileCountY - 1);

	std::uniform_real_distribution<float> AngleDist(0.f, std::numbers::pi_v<float> * 2.f);
	std::uniform_real_distribution<float> RadiusDist(0.f, 1.f);
	auto& RandEngine = CEngine::GetInst()->GetMT();

	constexpr int MaxTryCount = 24;
	for (int Try = 0; Try < MaxTryCount; ++Try)
	{
		const float Angle = AngleDist(RandEngine);
		const float Dist = std::sqrt(RadiusDist(RandEngine)) * Radius;

		const float SpawnX = Origin.x + std::cos(Angle) * Dist;
		const float SpawnY = Origin.y + std::sin(Angle) * Dist;

		if (SpawnX >= MinX && SpawnX <= MaxX && SpawnY >= MinY && SpawnY <= MaxY)
		{
			OutSpawnPos = FVector(SpawnX, SpawnY, Origin.z);
			return true;
		}
	}

	OutSpawnPos = FVector(std::clamp(Origin.x, MinX, MaxX), std::clamp(Origin.y, MinY, MaxY), Origin.z);
	return true;
}

void CBrotatoWorld_Battle::LoadAnimation2D()
{
	// TODO: 캐릭터 정보와 스테이지 정보를 읽어와 필요한 리소스만 읽기.

	const static FVector2 Zero = FVector2(0.f, 0.f);

	FVector2 TexSize{};
	std::vector<const TCHAR*> TexFileNames;

	auto CharacterBase = CharacterBaseTable::GetInst().Get();

	// 캐릭터 기본
	{
		const auto& PotatoBodyTexPath = CharacterBase->PotatoBodyTexPath;
		// 몸통
		WorldAssetManager->CreateAnimation(PotatoBodyTexPath);
		WorldAssetManager->SetAnimation2DTextureType(PotatoBodyTexPath, EAnimation2DTextureType::Frame);

		CA2T BodyName(PotatoBodyTexPath.c_str());
		WorldAssetManager->SetTexture(PotatoBodyTexPath, PotatoBodyTexPath, BodyName, Key::Path::Brotato);
		if (auto Tex = WorldAssetManager->FindTexture(PotatoBodyTexPath).lock())
		{
			TexSize.x = Tex->GetTexture()->Width;
			TexSize.y = Tex->GetTexture()->Height;
		}

		WorldAssetManager->AddFrame(PotatoBodyTexPath, Zero, TexSize);

		const auto& PotatoLegTexPath = CharacterBase->PotatoLegTexPath;
		// 다리
		WorldAssetManager->CreateAnimation(PotatoLegTexPath);
		WorldAssetManager->SetAnimation2DTextureType(PotatoLegTexPath, EAnimation2DTextureType::Frame);

		CA2T LegName(PotatoLegTexPath.c_str());
		WorldAssetManager->SetTexture(PotatoLegTexPath, PotatoLegTexPath, LegName, Key::Path::Brotato);
		if (auto Tex = WorldAssetManager->FindTexture(PotatoLegTexPath).lock())
		{
			TexSize.x = Tex->GetTexture()->Width;
			TexSize.y = Tex->GetTexture()->Height;
		}

		WorldAssetManager->AddFrame(PotatoLegTexPath, Zero, TexSize);
	}


	// 캐릭터
	for (const auto& Info : CharacterVisualTable::GetInst().GetItems() | std::views::values)
	{
		if (!Info->Eye.empty())
		{
			WorldAssetManager->CreateAnimation(Info->Eye);
			WorldAssetManager->SetAnimation2DTextureType(Info->Eye, EAnimation2DTextureType::Frame);

			CA2T EyeName(Info->Eye.c_str());
			WorldAssetManager->SetTexture(Info->Eye, Info->Eye, EyeName, Key::Path::Brotato);
			if (auto Tex = WorldAssetManager->FindTexture(Info->Eye).lock())
			{
				TexSize.x = Tex->GetTexture()->Width;
				TexSize.y = Tex->GetTexture()->Height;
			}

			WorldAssetManager->AddFrame(Info->Eye, Zero, TexSize);
		}

		if (!Info->Mouth.empty())
		{
			WorldAssetManager->CreateAnimation(Info->Mouth);
			WorldAssetManager->SetAnimation2DTextureType(Info->Mouth, EAnimation2DTextureType::Frame);

			CA2T MouthName(Info->Mouth.c_str());
			WorldAssetManager->SetTexture(Info->Mouth, Info->Mouth, MouthName, Key::Path::Brotato);
			if (auto Tex = WorldAssetManager->FindTexture(Info->Mouth).lock())
			{
				TexSize.x = Tex->GetTexture()->Width;
				TexSize.y = Tex->GetTexture()->Height;
			}

			WorldAssetManager->AddFrame(Info->Mouth, 1, Zero, TexSize);
		}

		for (const auto& Deco : Info->Decos)
		{
			if (!Deco.empty())
			{
				WorldAssetManager->CreateAnimation(Deco);
				WorldAssetManager->SetAnimation2DTextureType(Deco, EAnimation2DTextureType::Frame);

				CA2T DecoName(Deco.c_str());
				WorldAssetManager->SetTexture(Deco, Deco, DecoName, Key::Path::Brotato);
				if (auto Tex = WorldAssetManager->FindTexture(Deco).lock())
				{
					TexSize.x = Tex->GetTexture()->Width;
					TexSize.y = Tex->GetTexture()->Height;
				}

				WorldAssetManager->AddFrame(Deco, 1, Zero, TexSize);
			}
		}
	}

	// 몬스터
	auto MonsterPathView = EnemyTable::GetInst().GetItems()
		| std::views::values
		| std::views::transform([](const auto& Info) { return Info->SpritePath; })
		| std::views::filter([](const auto& Path) { return !Path.empty(); });

	for (const auto& Path : MonsterPathView)
	{
		WorldAssetManager->CreateAnimation(Path);
		WorldAssetManager->SetAnimation2DTextureType(Path, EAnimation2DTextureType::Frame);

		CA2T SpriteName(Path.c_str());
		WorldAssetManager->SetTexture(Path, Path, SpriteName, Key::Path::Brotato);
		if (auto Tex = WorldAssetManager->FindTexture(Path).lock())
		{
			TexSize.x = Tex->GetTexture()->Width;
			TexSize.y = Tex->GetTexture()->Height;
		}

		WorldAssetManager->AddFrame(Path, 1, Zero, TexSize);
	}

	// 무기
	auto WeaponPathView = WeaponTable::GetInst().GetItems()
		| std::views::values
		| std::views::transform([](const auto& Info) { return Info->SpritePath; })
		| std::views::filter([](const auto& Path) { return !Path.empty(); });

	for (const auto& Path : WeaponPathView)
	{
		WorldAssetManager->CreateAnimation(Path);
		WorldAssetManager->SetAnimation2DTextureType(Path, EAnimation2DTextureType::Frame);

		CA2T SpriteName(Path.c_str());
		WorldAssetManager->SetTexture(Path, Path, SpriteName, Key::Path::Brotato);
		if (auto Tex = WorldAssetManager->FindTexture(Path).lock())
		{
			TexSize.x = Tex->GetTexture()->Width;
			TexSize.y = Tex->GetTexture()->Height;
		}

		WorldAssetManager->AddFrame(Path, 1, Zero, TexSize);
	}

	// TODO: 아이템

	// 캐릭터 피격 이펙트
	for (int i = 0; i < 3; i++)
	{
		auto FileName = new TCHAR[MAX_PATH];
		wsprintf(FileName, TEXT("visual_effects/hit_effect/sprites/frame000%d.png"), i);
		TexFileNames.push_back(FileName);
	}

	WorldAssetManager->SetTextures(Key::Anim::HitEffect, Key::Anim::HitEffect, TexFileNames);

	for (auto& FileName : TexFileNames)
	{
		delete[] FileName;
	}
	TexFileNames.clear();

	// 투사체
	auto ProjectileView = ProjectileTable::GetInst().GetItems()
		| std::views::values
		| std::views::filter([](const auto& Info) { return !Info->SpritePaths.empty(); });

	for (const auto& Info : ProjectileView)
	{
		WorldAssetManager->CreateAnimation(Info->Name);
		WorldAssetManager->SetAnimation2DTextureType(Info->Name, EAnimation2DTextureType::Array);

		for (const auto& Path : Info->SpritePaths)
		{
			CA2T FileName(Path.c_str());
			TexFileNames.push_back(FileName);
		}

		WorldAssetManager->SetTextures(Info->Name, Info->Name, TexFileNames, Key::Path::Brotato);
		TexFileNames.clear();

		if (auto Tex = WorldAssetManager->FindTexture(Info->Name).lock())
		{
			TexSize.x = Tex->GetTexture()->Width;
			TexSize.y = Tex->GetTexture()->Height;
		}

		WorldAssetManager->AddFrame(Info->Name, Info->SpritePaths.size(), Zero, TexSize);
	}
}

void CBrotatoWorld_Battle::LoadSound()
{
}

void CBrotatoWorld_Battle::CreateUI()
{
}

void CBrotatoWorld_Battle::CreateTileMap()
{
	if (TileCountX <= 0 || TileCountY <= 0)
	{
		return;
	}

	constexpr float TileSize = 64.f;
	static constexpr std::array<const char*, 6> TopMaskFiles =
	{
		"mask_t0.png", "mask_t1.png", "mask_t2.png", "mask_t3.png", "mask_t4.png", "mask_t5.png"
	};
	static constexpr std::array<const char*, 6> BottomMaskFiles =
	{
		"mask_b0.png", "mask_b1.png", "mask_b2.png", "mask_b3.png", "mask_b4.png", "mask_b5.png"
	};
	static constexpr std::array<const char*, 6> LeftMaskFiles =
	{
		"mask_l0.png", "mask_l1.png", "mask_l2.png", "mask_l3.png", "mask_l4.png", "mask_l5.png"
	};
	static constexpr std::array<const char*, 6> RightMaskFiles =
	{
		"mask_r0.png", "mask_r1.png", "mask_r2.png", "mask_r3.png", "mask_r4.png", "mask_r5.png"
	};

	const int Theme = std::clamp(TileTheme, 1, 6);
	const float StartX = -TileCountX * TileSize * 0.5f + TileSize * 0.5f;
	const float StartY = -TileCountY * TileSize * 0.5f + TileSize * 0.5f;

	// 가장자리 충돌체
	{
		const float LeftEdgeX = StartX - TileSize;
		const float BotEdgeY = StartY - TileSize;
		const float Width = (TileCountX + 1) * TileSize;
		const float Height = (TileCountY + 1) * TileSize;

		// Top
		if (auto Col = EdgeColliders[0].lock())
		{
			Col->SetBoxExtent(Width, TileSize);
			Col->SetWorldPosition(TileSize * 0.5f, -BotEdgeY);
		}

		// Right
		if (auto Col = EdgeColliders[1].lock())
		{
			Col->SetBoxExtent(TileSize, Height);
			Col->SetWorldPosition(-LeftEdgeX, -TileSize * 0.5f);
		}

		// Bot
		if (auto Col = EdgeColliders[2].lock())
		{
			Col->SetBoxExtent(Width, TileSize);
			Col->SetWorldPosition(-TileSize * 0.5f, BotEdgeY);
		}

		// Left
		if (auto Col = EdgeColliders[3].lock())
		{
			Col->SetBoxExtent(TileSize, Height);
			Col->SetWorldPosition(LeftEdgeX, TileSize * 0.5f);
		}
	}

	std::uniform_int_distribution<int> EdgeMaskDist(0, 5);
	std::uniform_int_distribution<int> TileWeightDist(0, 99);
	std::uniform_int_distribution<int> NormalTileDist(0, 10);

	const int MaxX = TileCountX - 1;
	const int MaxY = TileCountY - 1;
	auto& RandEngine = CEngine::GetInst()->GetMT();

	for (int y = 0; y < TileCountY; ++y)
	{
		for (int x = 0; x < TileCountX; ++x)
		{
			auto WeakTile = CreateGameObject<CBrotatoTile>("BrotatoTile_" + std::to_string(x) + "_" + std::to_string(y));
			auto Tile = WeakTile.lock();
			if (!Tile)
			{
				continue;
			}

			Tile->SetWorldPosition(StartX + TileSize * x, StartY + TileSize * y);

			const int TileIndex = TileWeightDist(RandEngine) < 50 ? 11 : NormalTileDist(RandEngine);
			std::string TileFileName = std::format("resources/tiles/SingleTiles/{}/tile{:03}.png", Theme, TileIndex);

			std::string MaskPath = "resources/tiles/SingleTiles/mask/";
			if (x == 0 && y == 0)
			{
				MaskPath += "mask_bl.png";
			}
			else if (x == MaxX && y == 0)
			{
				MaskPath += "mask_br.png";
			}
			else if (x == 0 && y == MaxY)
			{
				MaskPath += "mask_tl.png";
			}
			else if (x == MaxX && y == MaxY)
			{
				MaskPath += "mask_tr.png";
			}
			else if (y == 0)
			{
				MaskPath += BottomMaskFiles[EdgeMaskDist(RandEngine)];
			}
			else if (y == MaxY)
			{
				MaskPath += TopMaskFiles[EdgeMaskDist(RandEngine)];
			}
			else if (x == 0)
			{
				MaskPath += LeftMaskFiles[EdgeMaskDist(RandEngine)];
			}
			else if (x == MaxX)
			{
				MaskPath += RightMaskFiles[EdgeMaskDist(RandEngine)];
			}
			else
			{
				MaskPath += "mask_center.png";
			}

			Tile->SetTileTextures(MaskPath, TileFileName, Key::Path::Brotato);
		}
	}
}

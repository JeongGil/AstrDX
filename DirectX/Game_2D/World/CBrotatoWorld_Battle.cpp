#include "CBrotatoWorld_Battle.h"

#include <array>
#include <algorithm>
#include <cmath>
#include <numbers>
#include <CEngine.h>

#include <Component/CMeshComponent.h>
#include <Component/CColliderBox2D.h>
#include <World/CWorldManager.h>

#include "CLoadingWorld.h"
#include "../Map/CBrotatoTile.h"
#include "../Strings.h"
#include "../Character/CCameraObject.h"
#include "../Character/CEnemy.h"
#include "../Character/CPlayerCharacter.h"
#include "../Inventory/CCharacterData.h"
#include "../Table/CharacterBaseTable.h"
#include "../Table/CharacterVisualTable.h"
#include "../Table/EnemyTable.h"
#include "../Table/MiscTable.h"
#include "../Table/ProjectileTable.h"
#include "../Table/WeaponTable.h"
#include "../UI/CBattleWidget.h"
#include "../UI/CResultWidget.h"
#include "../UI/CStageStateWidget.h"

bool CBrotatoWorld_Battle::Init()
{
	if (!CWorld::Init())
	{
		return false;
	}

	LoadAnimation2D();

	auto& CharacterData = CCharacterData::GetInst();

	PlayerCharacter = CreateGameObject<CPlayerCharacter>(Key::Obj::PC);
	if (auto PC = PlayerCharacter.lock())
	{
		PC->SetWorldPosition(FVector::Zero);
		PC->SetCharacterVisual(TableID(1));

		for (size_t i = 0; i < CharacterData.GetWeaponCount(); i++)
		{
			auto Weapon = CharacterData.GetWeapon(i);
			PC->AddWeapon(Weapon);
		}
	}

	// 임시
	EnemySpawnEntries.push_back({ TableID(1), 8.f, 0.f });
	EnemySpawnEntries.push_back({ TableID(2), 2.f, 2.f });
	EnemySpawnEntries.push_back({ TableID(3), 4.f, 4.f });

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

	CreateUI(PlayerCharacter);

	CharacterData.SetStageState(EStageState::Playing);

	return true;
}

void CBrotatoWorld_Battle::Update(const float DeltaTime)
{
	CWorld::Update(DeltaTime);

	auto StageState = CCharacterData::GetInst().GetStageState();
	switch (StageState)
	{
	case EStageState::None:
		break;
	case EStageState::Playing:
		RemainStageTime -= DeltaTime;
		break;
	case EStageState::Clear:
	case EStageState::Defeat:
		RemainFinishTime -= DeltaTime;
		break;
	}

	if (RemainFinishTime <= 0.f)
	{
		FinishStage(StageState == EStageState::Clear);
	}

	if (RemainStageTime <= 0.f)
	{
		EnableResultWidget(true);
		return;
	}

	if (auto PC = this->PlayerCharacter.lock())
	{
		if (PC->IsPendingDead())
		{
			EnableResultWidget(false);
			return;
		}
	}

	UpdateEnemySpawn(DeltaTime);
}

void CBrotatoWorld_Battle::EnableResultWidget(bool bClear) const
{
	CCharacterData::GetInst().SetStageState(bClear ? EStageState::Clear : EStageState::Defeat);

	if (auto BattleWidget = this->BattleWidget.lock())
	{
		if (auto Widget = BattleWidget->GetStageStateWidget().lock())
		{
			Widget->SetEnableStageResult(true);
			Widget->SetStageResultText(bClear);
		}
	}
}

void CBrotatoWorld_Battle::FinishStage(bool bClear)
{
	if (bClear)
	{
		if (auto World = CWorldManager::GetInst()->CreateWorld<CLoadingWorld>(true).lock())
		{
			World->Load(EWorldType::Shop);
		}
	}
	else
	{
		if (auto BattleWidget = this->BattleWidget.lock())
		{
			if (auto Widget = BattleWidget->GetBattleResultWidget().lock())
			{
				Widget->SetEnable(true);
			}
		}
	}
}

void CBrotatoWorld_Battle::UpdateEnemySpawn(float DeltaTime)
{
	if (EnemySpawnEntries.empty())
	{
		return;
	}

	auto Player = FindObjectOfType<CPlayerCharacter>().lock();
	if (!Player)
	{
		return;
	}

	const FVector PlayerPos = Player->GetWorldPosition();

	for (FEnemySpawnEntry& Entry : EnemySpawnEntries)
	{
		if (Entry.SpawnIntervalSec <= 0.f)
		{
			continue;
		}

		Entry.ElapsedTime += DeltaTime;
		while (Entry.ElapsedTime >= Entry.SpawnIntervalSec)
		{
			Entry.ElapsedTime -= Entry.SpawnIntervalSec;

			FVector SpawnPos;
			if (!TryGetEnemySpawnPosition(PlayerPos, EnemySpawnRadius, SpawnPos))
			{
				continue;
			}

			auto Enemy = CreateGameObject<CEnemy>("Monster_" + std::to_string(SpawnedEnemyCount++)).lock();
			if (!Enemy)
			{
				continue;
			}

			Enemy->SetWorldPosition(SpawnPos);
			Enemy->SetEnemyInfoID(Entry.EnemyID);
		}
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

	std::uniform_real_distribution<float> XDist(Origin.x - Radius, Origin.x + Radius);
	std::uniform_real_distribution<float> YDist(Origin.y - Radius, Origin.y + Radius);
	auto& RandEngine = CEngine::GetInst()->GetMT();

	const float RadiusSq = Radius * Radius;

	constexpr int MaxTryCount = 48;
	for (int Try = 0; Try < MaxTryCount; ++Try)
	{
		const float SpawnX = XDist(RandEngine);
		const float SpawnY = YDist(RandEngine);

		const float DiffX = SpawnX - Origin.x;
		const float DiffY = SpawnY - Origin.y;
		const float DistSq = DiffX * DiffX + DiffY * DiffY;

		const bool bInsideCircle = DistSq <= RadiusSq;
		const bool bInsideTileArea = SpawnX >= MinX && SpawnX <= MaxX && SpawnY >= MinY && SpawnY <= MaxY;

		if (bInsideCircle && bInsideTileArea)
		{
			OutSpawnPos = FVector(SpawnX, SpawnY, Origin.z);
			return true;
		}
	}

	return false;
}

void CBrotatoWorld_Battle::LoadAnimation2D()
{
	// TODO: 캐릭터 정보와 스테이지 정보를 읽어와 필요한 리소스만 읽기.

	const static FVector2 Zero = FVector2(0.f, 0.f);

	FVector2 TexSize{};
	std::vector<const TCHAR*> TexFileNames;
	std::vector<std::wstring> ProjectileWidePaths;

	auto CharacterBase = CharacterBaseTable::GetInst().Get();
	auto Misc = MiscTable::GetInst().Get();

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
	WorldAssetManager->CreateAnimation(Key::Anim::HitEffect);
	WorldAssetManager->SetAnimation2DTextureType(Key::Anim::HitEffect, EAnimation2DTextureType::Frame);

	std::vector<std::wstring> HitEffectWidePaths;
	HitEffectWidePaths.reserve(Misc->HitEffectTexPaths.size());

	for (const auto& Path : Misc->HitEffectTexPaths)
	{
		HitEffectWidePaths.emplace_back(Path.begin(), Path.end());
	}

	for (const auto& WidePath : HitEffectWidePaths)
	{
		TexFileNames.push_back(WidePath.c_str());
	}

	WorldAssetManager->SetTextures(Key::Anim::HitEffect, Key::Anim::HitEffect, TexFileNames, Key::Path::Brotato);
	TexFileNames.clear();

	if (auto Tex = WorldAssetManager->FindTexture(Key::Anim::HitEffect).lock())
	{
		TexSize.x = Tex->GetTexture()->Width;
		TexSize.y = Tex->GetTexture()->Height;
	}

	WorldAssetManager->AddFrame(Key::Anim::HitEffect, static_cast<int>(Misc->HitEffectTexPaths.size()), Zero, TexSize);

	TexFileNames.clear();

	// 투사체
	auto ProjectileView = ProjectileTable::GetInst().GetItems()
		| std::views::values
		| std::views::filter([](const auto& Info) { return !Info->SpritePaths.empty(); });

	for (const auto& Info : ProjectileView)
	{
		WorldAssetManager->CreateAnimation(Info->Name);
		WorldAssetManager->SetAnimation2DTextureType(Info->Name, EAnimation2DTextureType::Array);

		ProjectileWidePaths.clear();
		ProjectileWidePaths.reserve(Info->SpritePaths.size());
		for (const auto& Path : Info->SpritePaths)
		{
			ProjectileWidePaths.emplace_back(Path.begin(), Path.end());
		}

		for (const auto& WidePath : ProjectileWidePaths)
		{
			TexFileNames.push_back(WidePath.c_str());
		}

		WorldAssetManager->SetTextureArray(Info->Name, Info->Name, TexFileNames, Key::Path::Brotato);
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

void CBrotatoWorld_Battle::CreateUI(const std::weak_ptr<CPlayerCharacter>& PC)
{
	BattleWidget = UIManager->CreateWidget<CBattleWidget>("BattleWidget");
	if (auto Widget = BattleWidget.lock())
	{
		Widget->SetPlayerCharacter(PC);
	}
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

			const bool bIsEdgeTile = x == 0 || x == MaxX || y == 0 || y == MaxY;
			if (!bIsEdgeTile)
			{
				Tile->SetTileTexture(TileFileName, Key::Path::Brotato);
				continue;
			}

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
			else
			{
				MaskPath += RightMaskFiles[EdgeMaskDist(RandEngine)];
			}

			Tile->SetMaskAndTileTextures(MaskPath, TileFileName, Key::Path::Brotato);
		}
	}
}

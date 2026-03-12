#include "CBrotatoWorld_Battle.h"

#include <Asset/CPathManager.h>
#include <Component/CMeshComponent.h>
#include <Render/CRenderManager.h>

#include "../Strings.h"
#include "../Character/CEnemy.h"
#include "../Character/CPlayerCharacter.h"
#include "../Inventory/CInventoryData.h"
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

	CPathManager::CreatePath(Key::Path::Brotato, TEXT("Brotato\\"), Key::Path::Asset);

	CTableManager::GetInst().Init();
	CTableManager::GetInst().LoadTables();

	LoadAnimation2D();

	CInventoryData::GetInst().AddWeapon(TableID(1));
	//CInventoryData::GetInst().AddWeapon(TableID(1));
	//CInventoryData::GetInst().AddWeapon(TableID(1));

	auto WPC = CreateGameObject<CPlayerCharacter>(Key::Obj::PC);
	if (auto PC = WPC.lock())
	{
		PC->SetWorldPosition(0, 0);
		PC->SetCharacterVisual(TableID(2));

		for (size_t i = 0; i < CInventoryData::GetInst().GetWeaponCount(); i++)
		{
			auto Weapon = CInventoryData::GetInst().GetWeapon(i);
			PC->AddWeapon(Weapon);
		}
	}

	static int Counter = 0;
	auto WNPC = CreateGameObject<CEnemy>("Monster_" + std::to_string(Counter));
	if (auto NPC = WNPC.lock())
	{
		NPC->SetWorldPosition(300, 300);
		NPC->SetEnemyInfoID(TableID(1));
	}

	//WNPC = CreateGameObject<CEnemy>("Monster_" + std::to_string(Counter));
	//if (auto NPC = WNPC.lock())
	//{
	//	NPC->SetWorldPosition(0, 300);
	//	NPC->SetEnemyInfoID(TableID(2));
	//}

	//WNPC = CreateGameObject<CEnemy>("Monster_" + std::to_string(Counter));
	//if (auto NPC = WNPC.lock())
	//{
	//	NPC->SetWorldPosition(-300, 300);
	//}

	//WNPC = CreateGameObject<CEnemy>("Monster_" + std::to_string(Counter));
	//if (auto NPC = WNPC.lock())
	//{
	//	NPC->SetWorldPosition(300, -300);
	//}

	auto WBG = CreateGameObject<CGameObject>("BG");
	if (auto BG = WBG.lock())
	{
		auto WMesh = BG->CreateComponent<CMeshComponent>("BG");
		if (auto Mesh = WMesh.lock())
		{
			Mesh->SetShader("DefaultTexture2D");
			Mesh->SetMesh("CenterRectTex");
			Mesh->SetWorldScale(1024, 1024);

			Mesh->SetBlendState(0, "AlphaBlend");

			Mesh->AddTexture(0, "BG", TEXT("TestMap.png"), Key::Path::Brotato);

			Mesh->SetRenderLayer(ERenderOrder::Background);
		}
	}

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

void CBrotatoWorld_Battle::LoadAnimation2D()
{
	// TODO: 캐릭터 정보와 스테이지 정보를 읽어와 필요한 리소스만 읽기.

	const static FVector2 Zero = FVector2(0.f, 0.f);

	FVector2 TexSize{};
	std::vector<const TCHAR*> TexFileNames;

	// 캐릭터 기본
	{
		auto Info = MiscTable::GetInst().Get();

		// 몸통
		WorldAssetManager->CreateAnimation(Info->PotatoBodyTexPath);
		WorldAssetManager->SetAnimation2DTextureType(Info->PotatoBodyTexPath, EAnimation2DTextureType::Frame);

		CA2T BodyName(Info->PotatoBodyTexPath.c_str());
		WorldAssetManager->SetTexture(Info->PotatoBodyTexPath, Info->PotatoBodyTexPath, BodyName, Key::Path::Brotato);
		if (auto Tex = WorldAssetManager->FindTexture(Info->PotatoBodyTexPath).lock())
		{
			TexSize.x = Tex->GetTexture()->Width;
			TexSize.y = Tex->GetTexture()->Height;
		}

		WorldAssetManager->AddFrame(Info->PotatoBodyTexPath, Zero, TexSize);

		// 다리
		WorldAssetManager->CreateAnimation(Info->PotatoLegTexPath);
		WorldAssetManager->SetAnimation2DTextureType(Info->PotatoLegTexPath, EAnimation2DTextureType::Frame);

		CA2T LegName(Info->PotatoLegTexPath.c_str());
		WorldAssetManager->SetTexture(Info->PotatoLegTexPath, Info->PotatoLegTexPath, LegName, Key::Path::Brotato);
		if (auto Tex = WorldAssetManager->FindTexture(Info->PotatoLegTexPath).lock())
		{
			TexSize.x = Tex->GetTexture()->Width;
			TexSize.y = Tex->GetTexture()->Height;
		}

		WorldAssetManager->AddFrame(Info->PotatoLegTexPath, Zero, TexSize);
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

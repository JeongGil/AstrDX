#include "CBrotatoWorld_Battle.h"

#include <Asset/CPathManager.h>
#include <Component/CMeshComponent.h>
#include <Render/CRenderManager.h>

#include "../Strings.h"
#include "../Character/CNonPlayerCharacter.h"
#include "../Character/CPlayerCharacter.h"
#include "../Inventory/CInventory.h"
#include "../Table/CTableManager.h"

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

	CInventory::GetInst().AddWeapon(TableID(1));
	CInventory::GetInst().AddWeapon(TableID(1));
	CInventory::GetInst().AddWeapon(TableID(1));

	auto WPC = CreateGameObject<CPlayerCharacter>(Key::Obj::PC);
	if (auto PC = WPC.lock())
	{
		PC->SetWorldPosition(0, 0);
		PC->SetCharacterVisual(TableID(2));
		
		for (size_t i = 0; i< CInventory::GetInst().GetWeaponCount(); i++)
		{
			auto Weapon = CInventory::GetInst().GetWeapon(i);
			PC->AddWeapon(Weapon);
		}
	}

	/*static int Counter = 0;
	auto WNPC = CreateGameObject<CNonPlayerCharacter>("Monster_" + std::to_string(Counter));
	if (auto NPC = WNPC.lock())
	{
		NPC->SetWorldPosition(300, 300);
	}*/

	//WNPC = CreateGameObject<CNonPlayerCharacter>("Monster_" + std::to_string(Counter));
	//if (auto NPC = WNPC.lock())
	//{
	//	NPC->SetWorldPosition(-300, 300);
	//}

	//WNPC = CreateGameObject<CNonPlayerCharacter>("Monster_" + std::to_string(Counter));
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

			Mesh->TrySetRenderLayer(ERenderOrder::Background);
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

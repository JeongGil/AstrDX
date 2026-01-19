#include "CBrotatoWorld_Battle.h"

#include <Asset/CPathManager.h>
#include <Component/CMeshComponent.h>

#include "../Strings.h"
#include "../Character/CPlayerCharacter.h"
#include "../Table/CTableManager.h"

bool CBrotatoWorld_Battle::Init()
{
	if (!CWorld::Init())
	{
		return false;
	}

	CPathManager::CreatePath(Key::Path::Brotato, TEXT("Brotato\\"), Key::Path::Asset);

	CTableManager::GetInst().Init();
	CTableManager::GetInst().LoadTables();

	auto WPC = CreateGameObject<CPlayerCharacter>(Key::Obj::PC);
	if (auto PC = WPC.lock())
	{
		PC->SetWorldPosition(0, 0);
		PC->SetCharacterVisual(TableID(2));
	}

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

#include "CEditorWorld.h"

#include "../Player/CEditorPlayer.h"
#include "../Map/CTileMapMain.h"
#include "../UI/CEditorWidget.h"

CEditorWorld::CEditorWorld()
{
}

CEditorWorld::~CEditorWorld()
{

}

bool CEditorWorld::Init()
{
	if (!CWorld::Init())
	{
		return false;
	}

	LoadAnimation2D();
	LoadSound();
	CreateUI();

	Player = CreateGameObject<CEditorPlayer>("Player");
	TileMap = CreateGameObject<CTileMapMain>("TileMap");

	if (auto TileMapObj = TileMap.lock())
	{
		if (auto TileMap = TileMapObj->GetTileMap().lock())
		{
			TileMap->SetTileOutlineRender(true);
		}
	}

	return true;
}
void CEditorWorld::Update(const float DeltaTime)
{
	CWorld::Update(DeltaTime);
}

void CEditorWorld::LoadAnimation2D()
{

}

void CEditorWorld::LoadSound()
{

}

void CEditorWorld::CreateUI()
{
	auto Widget = UIManager->CreateWidget<CEditorWidget>("EditorWidget");
}

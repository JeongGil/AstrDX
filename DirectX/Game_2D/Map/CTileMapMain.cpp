#include "CTileMapMain.h"

CTileMapMain::CTileMapMain()
{
}

CTileMapMain::~CTileMapMain()
{
}

void CTileMapMain::Begin()
{
	CTileMapObject::Begin();
}

bool CTileMapMain::Init()
{
	if (!CTileMapObject::Init())
	{
		return false;
	}

	if (auto Render = TileMapRender.lock())
	{
		Render->EnableTileAlphaBlend();

		Render->SetTexture(ETileTextureType::Back, "MainMap", TEXT("MapBackGround.png"));

		Render->SetWorldScale(6400.f, 6400.f);

		Render->SetTexture(ETileTextureType::Tile, "MainTile", TEXT("Floors.png"));

		for (int i = 0; i < 5; ++i)
		{
			Render->AddTileFrame(0.f, i * 64.f,
				64.f, 64.f + i * 64.f);
		}
	}

	if (auto TileMap = this->TileMap.lock())
	{
		TileMap->CreateTile(ETileShape::Rect, 100, 100, FVector2(64.f, 64.f));

		TileMap->SetTileFrameAll(0);
	}

	return true;
}

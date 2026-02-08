#include "CTileMapObject.h"

CTileMapObject::CTileMapObject()
{
}

CTileMapObject::CTileMapObject(const CTileMapObject& other) :
	CGameObject(other),
	TileMapRender(other.TileMapRender),
	TileMap(other.TileMap)
{
}

CTileMapObject::CTileMapObject(CTileMapObject&& other) noexcept :
	CGameObject(std::move(other)),
	TileMapRender(std::move(other.TileMapRender)),
	TileMap(std::move(other.TileMap))
{
}

CTileMapObject::~CTileMapObject()
{
}

void CTileMapObject::Begin()
{
	CGameObject::Begin();
}

bool CTileMapObject::Init()
{
	if (!CGameObject::Init())
	{
		return false;
	}

	TileMapRender = CreateComponent<CTileMapRender>("Render");
	TileMap = CreateComponent<CTileMapComponent>("TileMap");

	return true;
}

void CTileMapObject::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CTileMapObject::PostUpdate(const float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CTileMapObject::Render()
{
	CGameObject::Render();
}

void CTileMapObject::Destroy()
{
	CGameObject::Destroy();
}

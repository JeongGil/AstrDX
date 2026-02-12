#pragma once
#include "CGameObject.h"

#include "../Component/CTileMapComponent.h"
#include "../Component/CTileMapRender.h"

class CWorld;

class CTileMapObject :
    public CGameObject
{
	friend CWorld;
	friend CObject;

public:
	CTileMapObject();
	CTileMapObject(const CTileMapObject& other);
	CTileMapObject(CTileMapObject&& other) noexcept;
	~CTileMapObject() override;

protected:
	std::weak_ptr<CTileMapRender> TileMapRender;
	std::weak_ptr<CTileMapComponent> TileMap;

public:
	void Begin() override;
	bool Init() override;
	void Update(float DeltaTime) override;
	void PostUpdate(const float DeltaTime) override;
	void Render() override;
	void Destroy() override;

	std::weak_ptr<CTileMapComponent> GetTileMap() const
	{
		return TileMap;
	}
};


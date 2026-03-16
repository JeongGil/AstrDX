#pragma once
#include <Object/CGameObject.h>

class CMeshComponent;
class CWorld;

class CBrotatoTile :
	public CGameObject
{
	friend CWorld;
	friend CObject;

protected:
	CBrotatoTile() = default;
	CBrotatoTile(const CBrotatoTile& Other) = default;
	CBrotatoTile(CBrotatoTile&& Other) noexcept = default;

public:
	~CBrotatoTile() override = default;

private:
	std::weak_ptr<CMeshComponent> Mesh;

public:
	bool Init() override;

	void SetTileTextures(const std::string& MaskTexturePath, const std::string& TileTexturePath,
		const std::string& PathName = "Brotato");

protected:
	CBrotatoTile* Clone() override;
};

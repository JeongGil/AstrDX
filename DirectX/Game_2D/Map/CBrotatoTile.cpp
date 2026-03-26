#include "CBrotatoTile.h"

#include <Component/CMeshComponent.h>

bool CBrotatoTile::Init()
{
	if (!CGameObject::Init())
	{
		return false;
	}

	Mesh = CreateComponent<CMeshComponent>("TileMesh");
	if (auto TileMesh = Mesh.lock())
	{
		TileMesh->SetShader("BrotatoTile");
		TileMesh->SetMesh("CenterRectTex");
		TileMesh->SetWorldScale(64.f, 64.f);
		TileMesh->SetBlendState(0, "AlphaBlend");
		TileMesh->SetRenderLayer(ERenderOrder::Map);
	}

	return true;
}

void CBrotatoTile::SetMaskAndTileTextures(const std::string& MaskTexturePath, const std::string& TileTexturePath,
	const std::string& PathName)
{
	auto TileMesh = Mesh.lock();
	if (!TileMesh)
	{
		return;
	}

	TileMesh->SetShader("BrotatoTile");
	TileMesh->ClearTextures(0);

	const std::string MaskTextureKey = "Mask_" + MaskTexturePath;
	const std::string TileTextureKey = "Tile_" + TileTexturePath;

	CA2T MaskFileName(MaskTexturePath.c_str());
	TileMesh->AddTexture(0, MaskTextureKey, MaskFileName, PathName, 0);

	CA2T TileFileName(TileTexturePath.c_str());
	TileMesh->AddTexture(0, TileTextureKey, TileFileName, PathName, 1);
}

void CBrotatoTile::SetTileTexture(const std::string& TileTexturePath, const std::string& PathName)
{
	auto TileMesh = Mesh.lock();
	if (!TileMesh)
	{
		return;
	}

	TileMesh->SetShader("DefaultTexture2D");
	TileMesh->ClearTextures(0);

	const std::string TileTextureKey = "InnerTile_" + TileTexturePath;

	CA2T TileFileName(TileTexturePath.c_str());
	TileMesh->AddTexture(0, TileTextureKey, TileFileName, PathName, 0);
}

CBrotatoTile* CBrotatoTile::Clone()
{
	return new CBrotatoTile(*this);
}

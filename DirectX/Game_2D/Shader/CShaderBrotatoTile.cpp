#include "CShaderBrotatoTile.h"

CShaderBrotatoTile::CShaderBrotatoTile()
{
	AssetType = EAssetType::Shader;
}

CShaderBrotatoTile::~CShaderBrotatoTile()
{
}

bool CShaderBrotatoTile::Init(const std::string& PathName)
{
	if (!LoadVertexShader("BrotatoTileVS", TEXT("BrotatoTile.hlsl"), PathName))
	{
		return false;
	}

	if (!LoadPixelShader("BrotatoTilePS", TEXT("BrotatoTile.hlsl"), PathName))
	{
		return false;
	}

	AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0);

	return CreateInputLayout();
}

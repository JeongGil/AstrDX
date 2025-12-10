#pragma once

#include "../../EngineInfo.h"

class CMesh;
class CAssetManager;

class CMeshManager
{
	friend CAssetManager;

public:
	bool Init();

	bool CreateMesh(const std::string Name, void* Vertices, int VertexSize, int VertexCount, D3D11_USAGE VertexUsage, D3D11_PRIMITIVE_TOPOLOGY Topology,
		void* Indices = nullptr, int IndexSize = 0, int IndexCount = 0, DXGI_FORMAT Format = DXGI_FORMAT_UNKNOWN, D3D11_USAGE IndexUsage = D3D11_USAGE_DEFAULT);

	CMesh* FindMesh(const std::string& Name) const;

private:
	std::unordered_map<std::string, std::shared_ptr<CMesh>> Meshes;

private:
	CMeshManager();

public:
	~CMeshManager();
};


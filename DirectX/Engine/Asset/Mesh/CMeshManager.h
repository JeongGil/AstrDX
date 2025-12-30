#pragma once

#include "../../EngineInfo.h"

class CMesh;
class CAssetManager;

class CMeshManager
{
	friend CAssetManager;

public:
	bool Init();

	void ReleaseAsset(const std::string& Key);

	/**
	 * @brief Initializes the mesh manager by creating predefined meshes for test.
	 *
	 * This method sets up essential meshes, such as a colored rectangle and a colored cube,
	 * by defining their vertices, indices, and associated properties. These meshes are
	 * stored and managed for later use in rendering operations.
	 *
	 * @return true if the initialization is successful and all meshes are created properly,
	 *         false otherwise.
	 */
	bool CreateMesh(const std::string& Key, void* Vertices, int VertexSize, int VertexCount, D3D11_USAGE VertexUsage, D3D11_PRIMITIVE_TOPOLOGY Topology,
		void* Indices = nullptr, int IndexSize = 0, int IndexCount = 0, DXGI_FORMAT Format = DXGI_FORMAT_UNKNOWN, D3D11_USAGE IndexUsage = D3D11_USAGE_DEFAULT);

	std::weak_ptr<CMesh> FindMesh(const std::string& Key) const;

private:
	std::unordered_map<std::string, std::shared_ptr<CMesh>> Meshes;

private:
	CMeshManager();

public:
	~CMeshManager();
};


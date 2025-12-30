#include "CMeshManager.h"

#include "CMesh.h"

bool CMeshManager::Init()
{
	// Create ColorMesh rectangle
	FVertexColor CenterRectColor[4] =
	{
		FVertexColor(-0.5f, 0.5f, 0.f, 1.f, 0.f, 0.f, 1.f),
		FVertexColor(0.5f, 0.5f, 0.f, 0.f, 1.f, 0.f, 1.f),
		FVertexColor(-0.5f, -0.5f, 0.f, 0.f, 0.f, 1.f, 1.f),
		FVertexColor(0.5f, -0.5f, 0.f, 1.f, 1.f, 0.f, 1.f)
	};

	unsigned short CenterRectColorIdx[6] = { 0, 1, 3, 0, 3, 2 };

	if (!CreateMesh("Mesh_CenterRectColor", CenterRectColor, sizeof(FVertexColor),
		4, D3D11_USAGE_IMMUTABLE, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		CenterRectColorIdx, 2, 6, DXGI_FORMAT_R16_UINT,
		D3D11_USAGE_IMMUTABLE))
	{
		return false;
	}

	// TextureMesh rectangle.
	FVertexTex CenterRectTexture[4] =
	{
		FVertexTex(-0.5f, 0.5f, 0.f, 0.f, 0.f),
		FVertexTex(0.5f, 0.5f, 0.f, 1.f, 0.f),
		FVertexTex(-0.5f, -0.5f, 0.f, 0.f, 1.f),
		FVertexTex(0.5f, -0.5f, 0.f, 1.f, 1.f)
	};

	if (!CreateMesh("Mesh_CenterRectTex", CenterRectTexture, sizeof(FVertexTex),
		4, D3D11_USAGE_IMMUTABLE, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		CenterRectColorIdx, 2, 6, DXGI_FORMAT_R16_UINT, D3D11_USAGE_IMMUTABLE))
	{
		return false;
	}

		FVertexColor CenterCubeColor[8] =
	{
		FVertexColor(-0.5f, 0.5f, -0.5f, 1.f, 0.f, 0.f, 1.f),
		FVertexColor(0.5f, 0.5f, -0.5f, 0.f, 1.f, 0.f, 1.f),
		FVertexColor(-0.5f, -0.5f, -0.5f, 0.f, 0.f, 1.f, 1.f),
		FVertexColor(0.5f, -0.5f, -0.5f, 1.f, 1.f, 0.f, 1.f),
		FVertexColor(-0.5f, 0.5f, 0.5f, 1.f, 0.f, 0.f, 1.f),
		FVertexColor(0.5f, 0.5f, 0.5f, 0.f, 1.f, 0.f, 1.f),
		FVertexColor(-0.5f, -0.5f, 0.5f, 0.f, 0.f, 1.f, 1.f),
		FVertexColor(0.5f, -0.5f, 0.5f, 1.f, 1.f, 0.f, 1.f)
	};

	unsigned short CenterCubeColorIdx[36] = { 0, 1, 3, 0, 3, 2,
		1, 5, 7, 1, 7, 3, 5, 4, 6, 5, 6, 7, 4, 0, 2, 4, 2, 6,
		4, 5, 1, 4, 1, 0, 2, 3, 7, 2, 7, 6 };

	if (!CreateMesh("Mesh_CenterCubeColor", CenterCubeColor,
		sizeof(FVertexColor),
		8, D3D11_USAGE_IMMUTABLE, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		CenterCubeColorIdx, 2, 36, DXGI_FORMAT_R16_UINT,
		D3D11_USAGE_IMMUTABLE))
	{
		return false;
	}

	return true;
}

void CMeshManager::ReleaseAsset(const std::string& Key)
{
	auto It = Meshes.find(Key);
	if (It != Meshes.end())
	{
		if (It->second.use_count() == 1)
		{
			Meshes.erase(It);
		}
	}
}

bool CMeshManager::CreateMesh(const std::string& Key, void* Vertices, int VertexSize, int VertexCount,
                              D3D11_USAGE VertexUsage, D3D11_PRIMITIVE_TOPOLOGY Topology, void* Indices, int IndexSize, int IndexCount,
                              DXGI_FORMAT Format, D3D11_USAGE IndexUsage)
{
	if (Meshes.contains(Key))
	{
		return true;
	}

	std::shared_ptr<CMesh> NewMesh(new CMesh);
	if (!NewMesh->CreateMesh(Vertices, VertexSize, VertexCount, VertexUsage, Topology,
		Indices, IndexSize, IndexCount, Format, IndexUsage))
	{
		return false;
	}

	NewMesh->SetName(Key);

	Meshes.emplace(Key, NewMesh);

	return true;
}

std::weak_ptr<CMesh> CMeshManager::FindMesh(const std::string& Key) const
{
	auto it = Meshes.find(Key);
	return it != Meshes.end() ? it->second : std::weak_ptr<CMesh>();
}

CMeshManager::CMeshManager()
{
}

CMeshManager::~CMeshManager()
{
}

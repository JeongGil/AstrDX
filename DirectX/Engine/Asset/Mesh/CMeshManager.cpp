#include "CMeshManager.h"

#include "CMesh.h"

bool CMeshManager::Init()
{
	// Create ColorMesh rectangle
	FVertexColor	CenterRectColor[4] =
	{
		FVertexColor(-0.5f, 0.5f, 0.f, 1.f, 0.f, 0.f, 1.f),
		FVertexColor(0.5f, 0.5f, 0.f, 0.f, 1.f, 0.f, 1.f),
		FVertexColor(-0.5f, -0.5f, 0.f, 0.f, 0.f, 1.f, 1.f),
		FVertexColor(0.5f, -0.5f, 0.f, 1.f, 1.f, 0.f, 1.f)
	};

	unsigned short	CenterRectColorIdx[6] = { 0, 1, 3, 0, 3, 2 };

	if (!CreateMesh("CenterRectColor", CenterRectColor, sizeof(FVertexColor),
		4, D3D11_USAGE_IMMUTABLE, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		CenterRectColorIdx, 2, 6, DXGI_FORMAT_R16_UINT,
		D3D11_USAGE_IMMUTABLE))
		return false;

	return true;
}

bool CMeshManager::CreateMesh(const std::string Name, void* Vertices, int VertexSize, int VertexCount,
	D3D11_USAGE VertexUsage, D3D11_PRIMITIVE_TOPOLOGY Topology, void* Indices, int IndexSize, int IndexCount,
	DXGI_FORMAT Format, D3D11_USAGE IndexUsage)
{
	if (Meshes.contains(Name))
	{
		return false;
	}

	auto NewMesh = std::shared_ptr<CMesh>(new CMesh);
	if (!NewMesh->CreateMesh(Vertices, VertexSize, VertexCount, VertexUsage, Topology,
		Indices, IndexSize, IndexCount, Format, IndexUsage))
	{
		return false;
	}

	Meshes.insert(std::make_pair(Name, NewMesh));

	return true;
}

CMesh* CMeshManager::FindMesh(const std::string& Name) const
{
	auto it = Meshes.find(Name);
	return it != Meshes.end() ? it->second.get() : nullptr;
}

CMeshManager::CMeshManager()
{
}

CMeshManager::~CMeshManager()
{
}

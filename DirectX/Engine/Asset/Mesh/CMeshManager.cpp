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

	FVector CenterFrameRect[4]
	{
		FVector(-0.5f, 0.5f, 0.f),
		FVector(0.5f, 0.5f, 0.f),
		FVector(-0.5f, -0.5f, 0.f),
		FVector(0.5f, -0.5f, 0.f)
	};

	unsigned short CenterFrameRectIdx[5]{ 0, 1, 3, 2, 0 };

	if (!CreateMesh("Mesh_CenterFrameRect", CenterFrameRect, sizeof(FVector),
		4, D3D11_USAGE_IMMUTABLE, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
		CenterFrameRectIdx, 2, 5, DXGI_FORMAT_R16_UINT,
		D3D11_USAGE_IMMUTABLE))
	{
		return false;
	}

	FVector LBFrameRect[4] =
	{
		FVector(0.f, 1.f, 0.f),
		FVector(1.f, 1.f, 0.f),
		FVector(0.f, 0.f, 0.f),
		FVector(1.f, 0.f, 0.f)
	};

	unsigned short LBFrameRectIdx[5]{ 0, 1, 3, 2, 0 };

	if (!CreateMesh("Mesh_LBFrameRect", LBFrameRect, sizeof(FVector),
		4, D3D11_USAGE_IMMUTABLE, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
		LBFrameRectIdx, 2, 5, DXGI_FORMAT_R16_UINT,
		D3D11_USAGE_IMMUTABLE))
	{
		return false;
	}

	std::vector<FVector> FrameSphere2D;
	for (int i = 0; i < 360; i += 12)
	{
		float Radian = DirectX::XMConvertToRadians(static_cast<float>(i));
		FrameSphere2D.emplace_back(cosf(Radian), sinf(Radian), 0.f);
	}

	std::vector<unsigned short> FrameSphere2DIdx;
	for (size_t i = 0; i < FrameSphere2D.size(); ++i)
	{
		FrameSphere2DIdx.push_back(static_cast<unsigned short>(i));
	}

	FrameSphere2DIdx.push_back(0);

	if (!CreateMesh("Mesh_FrameSphere2D", &FrameSphere2D[0], sizeof(FVector),
		static_cast<int>(FrameSphere2D.size()), D3D11_USAGE_IMMUTABLE,
		D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP, &FrameSphere2DIdx[0],
		2, static_cast<int>(FrameSphere2DIdx.size()),
		DXGI_FORMAT_R16_UINT, D3D11_USAGE_IMMUTABLE))
	{
		return false;
	}

	// TextureMesh rectangle.
	FVertexTex CenterRectTexture[4]
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

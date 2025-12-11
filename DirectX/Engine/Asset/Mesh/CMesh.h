#pragma once
#include "../../CObject.h"

struct FVertexBuffer
{
	ID3D11Buffer* Buffer = nullptr;

	// size of single vertex
	int Size = 0;
	int Count = 0;

	FVertexBuffer() = default;
	~FVertexBuffer()
	{
		if (Buffer)
		{
			Buffer->Release();
		}
	}
};

struct FIndexBuffer
{
	ID3D11Buffer* Buffer = nullptr;

	// size of single index
	int Size = 0;
	int Count = 0;

	// used to specify the index type when accessing or outputting using an index.
	DXGI_FORMAT Format = DXGI_FORMAT_UNKNOWN;

	FIndexBuffer() = default;
	~FIndexBuffer()
	{
		if (Buffer)
		{
			Buffer->Release();
		}
	}
};

struct FMeshSlot
{
	FIndexBuffer IndexBuffer;
	// Additional mesh slot data can be added here (e.g., material info)
};

class CMesh :
	public CObject
{
	friend class CMeshManager;

public:
	bool CreateMesh(void* Vertices, int VertexSize, int VertexCount, D3D11_USAGE VertexUsage, D3D11_PRIMITIVE_TOPOLOGY Topology,
		void* Indices = nullptr, int IndexSize = 0, int IndexCount = 0, DXGI_FORMAT Format = DXGI_FORMAT_UNKNOWN, D3D11_USAGE IndexUsage = D3D11_USAGE_DEFAULT);

	void Render() const;

protected:
	static bool CreateBuffer(ID3D11Buffer** Buffer, D3D11_BIND_FLAG Flag, void* Data, int Size, int Count, D3D11_USAGE Usage);

protected:
	FVertexBuffer VertexBuffer;
	std::vector<std::shared_ptr<FMeshSlot>> Slots;
	D3D11_PRIMITIVE_TOPOLOGY Topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

protected:
	CMesh();

public:
	~CMesh() override;
};


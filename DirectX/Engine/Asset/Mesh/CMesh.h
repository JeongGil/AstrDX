#pragma once
#include "../CAsset.h"

class CMaterial;

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
	std::shared_ptr<CMaterial> Material;
};

class CMesh :
	public CAsset
{
	friend class CMeshManager;

public:
	bool CreateMesh(void* VertexData, int VertexSize, int VertexCount, D3D11_USAGE VertexUsage,
		D3D11_PRIMITIVE_TOPOLOGY Topology, void* Indices = nullptr, int IndexSize = 0, int IndexCount = 0,
		DXGI_FORMAT Format = DXGI_FORMAT_UNKNOWN, D3D11_USAGE IndexUsage = D3D11_USAGE_DEFAULT);

	void Render() const;
	void Render(size_t SlotIndex);

	void SetMaterial(int SlotIndex);
	void SetMaterialBaseColor(int SlotIndex, float r, float g, float b, float a);
	void SetMaterialBaseColor(int SlotIndex, int r, int g, int b, int a);
	void SetMaterialBaseColor(int SlotIndex, const FVector4& Color);
	void SetMaterialOpacity(int SlotIndex, float Opacity);

public:
	FVector GetMin() const { return Min; }
	FVector GetMax() const { return Max; }
	FVector GetMeshSize() const { return MeshSize; }

	size_t GetSlotCount() const { return Slots.size(); }

	std::shared_ptr<FMeshSlot> GetSlot(size_t Index) const { return Slots[Index]; }

protected:
	static bool CreateBuffer(ID3D11Buffer** Buffer, D3D11_BIND_FLAG Flag, void* Data, int Size, int Count, D3D11_USAGE Usage);

protected:
	FVertexBuffer VertexBuffer;
	std::vector<std::shared_ptr<FMeshSlot>> Slots;
	D3D11_PRIMITIVE_TOPOLOGY Topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	FVector Min = FVector(FLT_MAX, FLT_MAX, FLT_MAX);
	FVector Max = FVector(FLT_MIN, FLT_MIN, FLT_MIN);
	FVector MeshSize;

protected:
	CMesh();

public:
	~CMesh() override;
};


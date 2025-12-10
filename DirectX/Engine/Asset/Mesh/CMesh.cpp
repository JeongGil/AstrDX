#include "CMesh.h"

#include "../../CDevice.h"

bool CMesh::CreateMesh(void* Vertices, int VertexSize, int VertexCount, D3D11_USAGE VertexUsage,
                       D3D11_PRIMITIVE_TOPOLOGY Topology, void* Indices, int IndexSize, int IndexCount, DXGI_FORMAT Format,
                       D3D11_USAGE IndexUsage)
{
	VertexBuffer.Size = VertexSize;
	VertexBuffer.Count = VertexCount;

	if (!CreateBuffer(&VertexBuffer.Buffer, D3D11_BIND_VERTEX_BUFFER, Vertices, VertexSize, VertexCount, VertexUsage))
	{
		return false;
	}

	if (Indices)
	{
		auto Slot = std::make_shared<FMeshSlot>();

		Slot->IndexBuffer.Size = IndexSize;
		Slot->IndexBuffer.Count = IndexCount;
		Slot->IndexBuffer.Format = Format;

		Slots.push_back(Slot);

		if (!CreateBuffer(&Slot->IndexBuffer.Buffer, D3D11_BIND_INDEX_BUFFER, Indices, IndexSize, IndexCount, IndexUsage))
		{
			return false;
		}
	}

	return true;
}

bool CMesh::CreateBuffer(ID3D11Buffer** Buffer, D3D11_BIND_FLAG Flag, void* Data, int Size, int Count,
	D3D11_USAGE Usage)
{
	UINT CPUAccessFlags = 0u;
	if (Usage == D3D11_USAGE_DYNAMIC)
	{
		CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else if (Usage == D3D11_USAGE_STAGING)
	{
		CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	}

	D3D11_BUFFER_DESC BufferDesc
	{
		.ByteWidth = static_cast<UINT>(Size * Count),
		.Usage = Usage,
		.BindFlags = static_cast<UINT>(Flag),
		.CPUAccessFlags = CPUAccessFlags,
	};

	D3D11_SUBRESOURCE_DATA BufferData{ .pSysMem = Data };

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&BufferDesc, &BufferData, Buffer)))
	{
		return false;
	}

	return true;
}

CMesh::CMesh()
{
}

CMesh::~CMesh()
{
}

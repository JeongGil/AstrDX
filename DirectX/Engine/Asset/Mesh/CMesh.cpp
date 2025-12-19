#include "CMesh.h"

#include "../../CDevice.h"
#include "../Material/CMaterial.h"

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

		SetMaterial(0);

		if (!CreateBuffer(&Slot->IndexBuffer.Buffer, D3D11_BIND_INDEX_BUFFER, Indices, IndexSize, IndexCount, IndexUsage))
		{
			return false;
		}
	}

	return true;
}

void CMesh::Render() const
{
	UINT Stride = VertexBuffer.Size;
	UINT Offset = 0;

	CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(Topology);
	CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 1, &VertexBuffer.Buffer, &Stride, &Offset);

	// No mesh slot (including index buffer).
	if (Slots.empty())
	{
		CDevice::GetInst()->GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
		CDevice::GetInst()->GetContext()->Draw(VertexBuffer.Count, 0);
	}
	else
	{
		for (auto Slot : Slots)
		{
			if (Slot->Material)
			{
				Slot->Material->UpdateConstantBuffer();
			}

			CDevice::GetInst()->GetContext()->IASetIndexBuffer(
				Slot->IndexBuffer.Buffer,
				Slot->IndexBuffer.Format, 0);

			CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 1, &VertexBuffer.Buffer, &Stride, &Offset);
			CDevice::GetInst()->GetContext()->DrawIndexed(Slot->IndexBuffer.Count, 0, 0);
		}
	}
}

void CMesh::Render(size_t SlotIndex)
{
	UINT Stride = VertexBuffer.Size;
	UINT Offset = 0;

	CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(Topology);
	CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 1, &VertexBuffer.Buffer, &Stride, &Offset);

	// No mesh slot (including index buffer).
	if (Slots.empty())
	{
		CDevice::GetInst()->GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
		CDevice::GetInst()->GetContext()->Draw(VertexBuffer.Count, 0);
	}
	else
	{
		const auto& Slot = Slots[SlotIndex];

		CDevice::GetInst()->GetContext()->IASetIndexBuffer(
			Slot->IndexBuffer.Buffer,
			Slot->IndexBuffer.Format, 0);

		CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 1, &VertexBuffer.Buffer, &Stride, &Offset);
		CDevice::GetInst()->GetContext()->DrawIndexed(Slot->IndexBuffer.Count, 0, 0);
	}
}

void CMesh::SetMaterial(int SlotIndex)
{
	if (!Slots[SlotIndex]->Material)
	{
		Slots[SlotIndex]->Material.reset(new CMaterial);
		Slots[SlotIndex]->Material->Init();
	}
}

void CMesh::SetMaterialBaseColor(int SlotIndex, float r, float g, float b, float a)
{
	if (!Slots[SlotIndex]->Material)
	{
		Slots[SlotIndex]->Material.reset(new CMaterial);
		Slots[SlotIndex]->Material->Init();
	}

	Slots[SlotIndex]->Material->SetBaseColor(r, g, b, a);
}

void CMesh::SetMaterialBaseColor(int SlotIndex, int r, int g, int b, int a)
{
	if (!Slots[SlotIndex]->Material)
	{
		Slots[SlotIndex]->Material.reset(new CMaterial);
		Slots[SlotIndex]->Material->Init();
	}

	Slots[SlotIndex]->Material->SetBaseColor(r, g, b, a);
}

void CMesh::SetMaterialBaseColor(int SlotIndex, const FVector4& Color)
{
	if (!Slots[SlotIndex]->Material)
	{
		Slots[SlotIndex]->Material.reset(new CMaterial);
		Slots[SlotIndex]->Material->Init();
	}

	Slots[SlotIndex]->Material->SetBaseColor(Color);
}

void CMesh::SetMaterialOpacity(int SlotIndex, float Opacity)
{
	if (!Slots[SlotIndex]->Material)
	{
		Slots[SlotIndex]->Material.reset(new CMaterial);
		Slots[SlotIndex]->Material->Init();
	}

	Slots[SlotIndex]->Material->SetOpacity(Opacity);
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

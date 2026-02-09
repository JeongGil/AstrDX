#include "CMesh.h"

#include "../../CDevice.h"
#include "../Material/CMaterial.h"

bool CMesh::CreateMesh(void* VertexData, int VertexSize, int VertexCount, D3D11_USAGE VertexUsage,
	D3D11_PRIMITIVE_TOPOLOGY Topology, void* Indices, int IndexSize, int IndexCount, DXGI_FORMAT Format,
	D3D11_USAGE IndexUsage)
{
	VertexBuffer.Size = VertexSize;
	VertexBuffer.Count = VertexCount;
	this->Topology = Topology;

	if (!CreateBuffer(&VertexBuffer.Buffer, D3D11_BIND_VERTEX_BUFFER, VertexData, VertexSize, VertexCount, VertexUsage))
	{
		return false;
	}

	auto Vertices = (char*)VertexData;
	for (int i = 0; i < VertexCount; ++i)
	{
		FVector Position = *((FVector*)Vertices);
		Min.x = min(Min.x, Position.x);
		Min.y = min(Min.y, Position.y);
		Min.z = min(Min.z, Position.z);
		Max.x = max(Max.x, Position.x);
		Max.y = max(Max.y, Position.y);
		Max.z = max(Max.z, Position.z);

		Vertices += VertexSize;
	}

	MeshSize = Max - Min;

	if (Indices)
	{
		auto Slot = std::make_shared<FMeshSlot>();

		Slot->IndexBuffer.Size = IndexSize;
		Slot->IndexBuffer.Count = IndexCount;
		Slot->IndexBuffer.Format = Format;

		MeshSlots.push_back(Slot);

		SetMaterial(0);

		if (!CreateBuffer(&Slot->IndexBuffer.Buffer, D3D11_BIND_INDEX_BUFFER, Indices, IndexSize, IndexCount, IndexUsage))
		{
			return false;
		}
	}

	return true;
}

bool CMesh::CreateInstancingBuffer(int Size, int Count)
{
	SAFE_RELEASE(InstancingBuffer.Buffer);

	InstancingBuffer.Size = Size;
	InstancingBuffer.Count = Count;

	D3D11_BUFFER_DESC BufferDesc{};
	BufferDesc.ByteWidth = Size * Count;
	BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&BufferDesc, nullptr, &InstancingBuffer.Buffer)))
	{
		return false;
	}

	return  true;
}

bool CMesh::SetInstancingData(void* Data, int Count)
{
	if (!InstancingBuffer.Buffer)
	{
		return false;
	}

	if (InstancingBuffer.Count < Count)
	{
		if (!CreateInstancingBuffer(InstancingBuffer.Size, Count * 2))
		{
			return false;
		}
	}

	ID3D11DeviceContext* Context = CDevice::GetInst()->GetContext();

	D3D11_MAPPED_SUBRESOURCE MS = {};

	Context->Map(InstancingBuffer.Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MS);

	memcpy(MS.pData, Data, InstancingBuffer.Size * Count);

	Context->Unmap(InstancingBuffer.Buffer, 0);

	InstancingCount = Count;

	return true;
}

void CMesh::RenderInstancing()
{
	if (InstancingCount == 0)
	{
		return;
	}

	ID3D11DeviceContext* Context =
		CDevice::GetInst()->GetContext();

	Context->IASetPrimitiveTopology(Topology);

	ID3D11Buffer* VB[2] =
	{
		VertexBuffer.Buffer,
		InstancingBuffer.Buffer
	};

	UINT Stride[2] = { VertexBuffer.Size, InstancingBuffer.Size };
	UINT Offset[2] = {};

	Context->IASetVertexBuffers(0, 2, VB, Stride, Offset);

	if (MeshSlots.empty())
	{
		Context->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
		Context->DrawInstanced(VertexBuffer.Count, InstancingCount, 0, 0);
	}
	else
	{
		for (const auto& MeshSlot : MeshSlots)
		{
			if (MeshSlot->Material)
			{
				MeshSlot->Material->UpdateConstantBuffer();
			}

			Context->IASetIndexBuffer(MeshSlot->IndexBuffer.Buffer, MeshSlot->IndexBuffer.Format, 0);			// 인덱스 버퍼의 인덱스를 참고하여 화면에 도형을 출력한다.
			Context->DrawIndexedInstanced(MeshSlot->IndexBuffer.Count, InstancingCount, 0, 0, 0);
		}
	}
}

void CMesh::RenderInstancing(size_t SlotIndex)
{
	if (InstancingCount == 0)
	{
		return;
	}

	ID3D11DeviceContext* Context = CDevice::GetInst()->GetContext();

	Context->IASetPrimitiveTopology(Topology);

	ID3D11Buffer* VB[2] =
	{
		VertexBuffer.Buffer,
		InstancingBuffer.Buffer
	};

	UINT Stride[2] = { VertexBuffer.Size, InstancingBuffer.Size };
	UINT Offset[2] = {};

	Context->IASetVertexBuffers(0, 2, VB, Stride, Offset);
	if (MeshSlots.empty())
	{
		Context->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);		Context->DrawInstanced(VertexBuffer.Count, InstancingCount,
			0, 0);
	}

	else
	{
		const auto& MeshSlot = MeshSlots[SlotIndex];
		if (MeshSlot->Material)
		{
			MeshSlot->Material->UpdateConstantBuffer();
		}

		Context->IASetIndexBuffer(MeshSlot->IndexBuffer.Buffer, MeshSlot->IndexBuffer.Format, 0);
		Context->DrawIndexedInstanced(MeshSlot->IndexBuffer.Count, InstancingCount, 0, 0, 0);
	}
}

void CMesh::Render() const
{
	UINT Stride = VertexBuffer.Size;
	UINT Offset = 0;

	CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(Topology);
	CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 1, &VertexBuffer.Buffer, &Stride, &Offset);

	// No mesh slot (including index buffer).
	if (MeshSlots.empty())
	{
		CDevice::GetInst()->GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
		CDevice::GetInst()->GetContext()->Draw(VertexBuffer.Count, 0);
	}
	else
	{
		for (const auto& Slot : MeshSlots)
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
	if (MeshSlots.empty())
	{
		CDevice::GetInst()->GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
		CDevice::GetInst()->GetContext()->Draw(VertexBuffer.Count, 0);
	}
	else
	{
		const auto& Slot = MeshSlots[SlotIndex];

		CDevice::GetInst()->GetContext()->IASetIndexBuffer(
			Slot->IndexBuffer.Buffer,
			Slot->IndexBuffer.Format, 0);

		CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 1, &VertexBuffer.Buffer, &Stride, &Offset);
		CDevice::GetInst()->GetContext()->DrawIndexed(Slot->IndexBuffer.Count, 0, 0);
	}
}

void CMesh::SetMaterial(int SlotIndex)
{
	if (!MeshSlots[SlotIndex]->Material)
	{
		MeshSlots[SlotIndex]->Material.reset(new CMaterial);
		MeshSlots[SlotIndex]->Material->Init();
	}
}

void CMesh::SetMaterialBaseColor(int SlotIndex, float r, float g, float b, float a)
{
	if (!MeshSlots[SlotIndex]->Material)
	{
		MeshSlots[SlotIndex]->Material.reset(new CMaterial);
		MeshSlots[SlotIndex]->Material->Init();
	}

	MeshSlots[SlotIndex]->Material->SetBaseColor(r, g, b, a);
}

void CMesh::SetMaterialBaseColor(int SlotIndex, int r, int g, int b, int a)
{
	if (!MeshSlots[SlotIndex]->Material)
	{
		MeshSlots[SlotIndex]->Material.reset(new CMaterial);
		MeshSlots[SlotIndex]->Material->Init();
	}

	MeshSlots[SlotIndex]->Material->SetBaseColor(r, g, b, a);
}

void CMesh::SetMaterialBaseColor(int SlotIndex, const FVector4& Color)
{
	if (!MeshSlots[SlotIndex]->Material)
	{
		MeshSlots[SlotIndex]->Material.reset(new CMaterial);
		MeshSlots[SlotIndex]->Material->Init();
	}

	MeshSlots[SlotIndex]->Material->SetBaseColor(Color);
}

void CMesh::SetMaterialOpacity(int SlotIndex, float Opacity)
{
	if (!MeshSlots[SlotIndex]->Material)
	{
		MeshSlots[SlotIndex]->Material.reset(new CMaterial);
		MeshSlots[SlotIndex]->Material->Init();
	}

	MeshSlots[SlotIndex]->Material->SetOpacity(Opacity);
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
	AssetType = EAssetType::Mesh;
}

CMesh::~CMesh()
{
}

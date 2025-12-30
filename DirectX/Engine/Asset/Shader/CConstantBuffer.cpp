#include "CConstantBuffer.h"

#include "../../CDevice.h"
#include "CBufferContainer.h"

bool CConstantBuffer::Init(int Size, int Register, int ShaderBuffer)
{
	this->Size = Size;
	this->Register = Register;
	this->ShaderBuffer = ShaderBuffer;

	D3D11_BUFFER_DESC Desc
	{
		.ByteWidth = static_cast<UINT>(Size),
		// Constant buffers are created as Dynamic because their data is frequently updated.
		.Usage = D3D11_USAGE_DYNAMIC,
		.BindFlags = D3D11_BIND_CONSTANT_BUFFER,
		.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
	};

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&Desc, nullptr, &Buffer)))
	{
		return false;
	}

	return true;
}

void CConstantBuffer::Update(const void* Data) const
{
	D3D11_MAPPED_SUBRESOURCE Map = {};

	// Retrieves the starting address of the buffer array into the pData variable inside the Map.
	CDevice::GetInst()->GetContext()->Map(Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &Map);

	memcpy(Map.pData, Data, Size);

	CDevice::GetInst()->GetContext()->Unmap(Buffer, 0);

	// Vertex
	if ((ShaderBuffer & EShaderBufferType::Vertex) != 0)
	{
		CDevice::GetInst()->GetContext()->VSSetConstantBuffers(Register, 1, &Buffer);
	}

	// Pixel
	if ((ShaderBuffer & EShaderBufferType::Pixel) != 0)
	{
		CDevice::GetInst()->GetContext()->PSSetConstantBuffers(Register, 1, &Buffer);
	}

	// Domain
	if ((ShaderBuffer & EShaderBufferType::Domain) != 0)
	{
		CDevice::GetInst()->GetContext()->DSSetConstantBuffers(Register, 1, &Buffer);
	}

	// Hull
	if ((ShaderBuffer & EShaderBufferType::Hull) != 0)
	{
		CDevice::GetInst()->GetContext()->HSSetConstantBuffers(Register, 1, &Buffer);
	}

	// Geometry
	if ((ShaderBuffer & EShaderBufferType::Geometry) != 0)
	{
		CDevice::GetInst()->GetContext()->GSSetConstantBuffers(Register, 1, &Buffer);
	}

	// Compute
	if ((ShaderBuffer & EShaderBufferType::Compute) != 0)
	{
		CDevice::GetInst()->GetContext()->CSSetConstantBuffers(Register, 1, &Buffer);
	}
}

CConstantBuffer::CConstantBuffer()
{
	AssetType = EAssetType::ConstantBuffer;
}

CConstantBuffer::~CConstantBuffer()
{
	SAFE_RELEASE(Buffer);
}

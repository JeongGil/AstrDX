#include "CCBufferAnimation2D.h"

#include "CConstantBuffer.h"

bool CCBufferAnimation2D::Init()
{
	SetConstantBuffer("CBuffer_Animation2D");

	return true;
}

void CCBufferAnimation2D::UpdateBuffer()
{
	if (auto Buffer = this->Buffer.lock())
	{
		Buffer->Update(&Data);
	}
}

CConstantBufferData* CCBufferAnimation2D::Clone()
{
	return new CCBufferAnimation2D(*this);
}

CCBufferAnimation2D::CCBufferAnimation2D(const CCBufferAnimation2D& other)
	: CConstantBufferData(other),
	Data(other.Data)
{
}

CCBufferAnimation2D::CCBufferAnimation2D(CCBufferAnimation2D&& other) noexcept
	: CConstantBufferData(std::move(other)),
	Data(std::move(other.Data))
{
}

CCBufferAnimation2D& CCBufferAnimation2D::operator=(const CCBufferAnimation2D& other)
{
	if (this == &other)
		return *this;
	CConstantBufferData::operator =(other);
	Data = other.Data;
	return *this;
}

CCBufferAnimation2D& CCBufferAnimation2D::operator=(CCBufferAnimation2D&& other) noexcept
{
	if (this == &other)
		return *this;
	CConstantBufferData::operator =(std::move(other));
	Data = std::move(other.Data);
	return *this;
}

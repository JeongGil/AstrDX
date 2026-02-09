#include "CCBufferBlur.h"

#include "CConstantBuffer.h"

CCBufferBlur::CCBufferBlur()
{
}

CCBufferBlur::~CCBufferBlur()
{
}

bool CCBufferBlur::Init()
{
	SetConstantBuffer("Blur");

	return true;
}

void CCBufferBlur::UpdateBuffer()
{
	if (auto CBuffer = Buffer.lock())
	{
		CBuffer->Update(&Data);
	}
}

CCBufferBlur* CCBufferBlur::Clone()
{
	return new CCBufferBlur(*this);
}

CCBufferBlur::CCBufferBlur(const CCBufferBlur& other) :
	CConstantBufferData(other),
	Data(other.Data)
{
}

CCBufferBlur::CCBufferBlur(CCBufferBlur&& other) noexcept :
	CConstantBufferData(std::move(other)),
	Data(std::move(other.Data))
{
}

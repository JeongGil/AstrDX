#include "CCBufferUIDefault.h"

#include "CConstantBuffer.h"

CCBufferUIDefault::CCBufferUIDefault()
{
}

CCBufferUIDefault::~CCBufferUIDefault()
{
}

bool CCBufferUIDefault::Init()
{
	SetConstantBuffer("UIDefault");

	return true;
}

void CCBufferUIDefault::UpdateBuffer()
{
	if (auto CBuffer = Buffer.lock())
	{
		CBuffer->Update(&Data);
	}
}

CCBufferUIDefault* CCBufferUIDefault::Clone()
{
	return new CCBufferUIDefault(*this);
}

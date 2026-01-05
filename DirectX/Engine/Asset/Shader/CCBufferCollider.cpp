#include "CCBufferCollider.h"

#include "CConstantBuffer.h"

bool CCBufferCollider::Init()
{
	SetConstantBuffer("Collider");
	return true;
}

void CCBufferCollider::UpdateBuffer()
{
	if (auto ConstantBuffer = Buffer.lock())
	{
		ConstantBuffer->Update(&Data);
	}
}

CCBufferCollider* CCBufferCollider::Clone()
{
	return new CCBufferCollider(*this);
}

#include "CCBufferMaterial.h"

#include "CConstantBuffer.h"

bool CCBufferMaterial::Init()
{
	SetConstantBuffer("Material");

	return true;
}

void CCBufferMaterial::UpdateBuffer()
{
	if (auto Buffer = this->Buffer.lock())
	{
		Buffer->Update(&Data);
	}
}

CCBufferMaterial* CCBufferMaterial::Clone()
{
	return new CCBufferMaterial(*this);
}

#include "CCBufferHit.h"

#include <Asset/CAssetManager.h>
#include <Asset/Shader/CConstantBuffer.h>
#include <Asset/Shader/CShaderManager.h>

CCBufferHit::CCBufferHit()
{
}

CCBufferHit::CCBufferHit(const CCBufferHit& Other) :
	CConstantBufferData(Other),
	Data(Other.Data)
{
}

CCBufferHit::CCBufferHit(CCBufferHit&& Other) noexcept :
	CConstantBufferData(std::move(Other)),
	Data(std::move(Other.Data))
{
}

CCBufferHit::~CCBufferHit()
{
}

bool CCBufferHit::Init()
{
	if (auto ShaderMgr = CAssetManager::GetInst()->GetShaderManager().lock())
	{
		ShaderMgr->CreateCBuffer("Hit", sizeof(FCBufferHitData), 10);

		SetConstantBuffer("Hit");
	}

	return true;
}

void CCBufferHit::UpdateBuffer()
{
	if (auto CBuffer = Buffer.lock())
	{
		CBuffer->Update(&Data);
	}
}

CCBufferHit* CCBufferHit::Clone()
{
	return new CCBufferHit(*this);
}

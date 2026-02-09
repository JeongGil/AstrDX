#include "CPostProcessHit.h"

#include <Asset/CAssetManager.h>
#include <Asset/Texture/CRenderTarget.h>
#include <Asset/Texture/CTextureManager.h>
#include <Render/CRenderManager.h>

#include "../Shader/CCBufferHit.h"

CPostProcessHit::CPostProcessHit()
{
}

CPostProcessHit::~CPostProcessHit()
{
}

bool CPostProcessHit::Init()
{
	if (!CPostProcess::Init())
	{
		return false;
	}

	HitCBuffer.reset(new CCBufferHit);
	HitCBuffer->Init();

	CreateRenderTarget(DXGI_FORMAT_R8G8B8A8_UNORM, true);

	SetBlendTargetInfo(0);

	HitColor.x = 1.f;
	HitColor.w = 1.f;

	HitCBuffer->SetColor(HitColor);

	SetShader("Hit");
	SetOrder(20);
	//SetBlendState("AlphaBlend");

	if (auto TexMgr = CAssetManager::GetInst()->GetTextureManager().lock())
	{
		TexMgr->LoadTexture("Hit", TEXT("TestEffect.png"));

		HitTexture = TexMgr->FindTexture("Hit");
	}

	return true;
}

void CPostProcessHit::Update(const float DeltaTime)
{
	Elapsed += DeltaTime;
	if (Elapsed >= Duration)
	{
		Elapsed = 0.f;
		SetEnable(false);
	}
}

void CPostProcessHit::Render()
{
	if (auto HitTex = HitTexture.lock())
	{
		HitTex->SetShader(1, EShaderBufferType::Pixel, 0);
	}

	HitCBuffer->UpdateBuffer();
}

void CPostProcessHit::SetHitColor(const FColor& Color)
{
	HitColor = Color;
	HitCBuffer->SetColor(Color);
}

void CPostProcessHit::SetHitColor(float r, float g, float b, float a)
{
	SetHitColor(FColor(r, g, b, a));
}

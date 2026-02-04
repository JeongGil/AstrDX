#include "CPostProcessHit.h"

#include <Asset/CAssetManager.h>
#include <Asset/Texture/CRenderTarget.h>
#include <Asset/Texture/CTextureManager.h>
#include <Render/CRenderManager.h>

CPostProcessHit::CPostProcessHit()
{
}

CPostProcessHit::~CPostProcessHit()
{
}

bool CPostProcessHit::Init()
{
	SetShader("Hit");

	MainTarget = CRenderManager::GetInst()->FindRenderTarget("MainTarget");

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
	if (Elapsed >= EnableTime)
	{
		Elapsed = 0.f;
		SetEnable(false);
	}
}

void CPostProcessHit::Render()
{
	if (auto Target = this->Target.lock())
	{
		Target->SetShader(0, EShaderBufferType::Pixel, 0);
	}

	if (auto HitTex = HitTexture.lock())
	{
		HitTex->SetShader(1, EShaderBufferType::Pixel, 0);
	}
}

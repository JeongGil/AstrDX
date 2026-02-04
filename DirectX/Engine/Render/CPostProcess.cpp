#include "CPostProcess.h"

#include "CRenderManager.h"
#include "CRenderState.h"
#include "../CDevice.h"
#include "../Asset/CAssetManager.h"
#include "../Asset/Shader/CShaderManager.h"
#include "../Asset/Texture/CRenderTarget.h"

CPostProcess::CPostProcess()
{
}

CPostProcess::~CPostProcess()
{
}

bool CPostProcess::Init()
{
	SetShader("NullBuffer");

	return true;
}

void CPostProcess::RenderFullScreenQuad()
{
	if (auto Shader = this->Shader.lock())
	{
		Shader->SetShader();
	}

	auto Context = CDevice::GetInst()->GetContext();

	UINT Offset{ 0 };
	Context->IASetVertexBuffers(0, 0, nullptr, nullptr, &Offset);
	Context->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	Context->Draw(4, 0);
}

void CPostProcess::RenderPostProcess()
{
	if (Target)
	{
		Target->SetTarget();
	}

	if (BlendState)
	{
		BlendState->SetState();
	}

	Render();
	RenderFullScreenQuad();

	if (BlendState)
	{
		BlendState->ResetState();
	}

	if (Target)
	{
		Target->ResetTarget();
	}
}

void CPostProcess::SetShader(const std::string& Key)
{
	if (auto ShaderMgr = CAssetManager::GetInst()->GetShaderManager().lock())
	{
		Shader = ShaderMgr->FindShader(Key);
	}
}

void CPostProcess::CreateRenderTarget(DXGI_FORMAT Format, bool bEnableDepth)
{
	FResolution RS = CDevice::GetInst()->GetResolution();

	Target = CRenderTarget::Create(Key, RS.Width, RS.Height, Format, bEnableDepth);
}

void CPostProcess::SetBlendState(const std::string& Key)
{
	BlendState = CRenderManager::GetInst()->FindRenderState(Key).lock();
}

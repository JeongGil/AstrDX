#include "CPostProcess.h"

#include "CRenderManager.h"
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
	Target = CRenderManager::GetInst()->FindRenderTarget("FinalTarget");

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
	if (auto Target = this->Target.lock())
	{
		Target->SetTarget();

		Render();
		RenderFullScreenQuad();

		Target->ResetTarget();
	}
}

void CPostProcess::SetRenderTarget(const std::weak_ptr<CRenderTarget>& Target)
{
	this->Target = Target;
}

void CPostProcess::SetRenderTarget(const std::string& Key)
{
	SetRenderTarget(CRenderManager::GetInst()->FindRenderTarget(Key));
}

void CPostProcess::SetShader(const std::string& Key)
{
	if (auto ShaderMgr = CAssetManager::GetInst()->GetShaderManager().lock())
	{
		Shader = ShaderMgr->FindShader(Key);
	}
}

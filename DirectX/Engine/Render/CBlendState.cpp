#include "CBlendState.h"

#include <ranges>

#include "../CDevice.h"

void CBlendState::SetState()
{
	CDevice::GetInst()->GetContext()->OMGetBlendState((ID3D11BlendState**)&PrevState, PrevBlendFactor, &PrevSampleMask);
	CDevice::GetInst()->GetContext()->OMSetBlendState((ID3D11BlendState*)State, BlendFactor, SampleMask);
}

void CBlendState::ResetState()
{
	CDevice::GetInst()->GetContext()->OMSetBlendState((ID3D11BlendState*)PrevState, PrevBlendFactor, PrevSampleMask);

	SAFE_RELEASE(PrevState);
}

void CBlendState::AddRenderTargetDesc(bool BlendEnable, D3D11_BLEND SrcBlend, D3D11_BLEND DestBlend,
                                      D3D11_BLEND_OP BlendOp, D3D11_BLEND SrcBlendAlpha, D3D11_BLEND DestBlendAlpha, D3D11_BLEND_OP BlendOpAlpha, UINT8
                                      RenderTargetWriteMask)
{
	TargetDescs.emplace_back(
		BlendEnable,
		SrcBlend,
		DestBlend,
		BlendOp,
		SrcBlendAlpha,
		DestBlendAlpha,
		BlendOpAlpha,
		RenderTargetWriteMask);
}

void CBlendState::SetBlendFactor(float r, float g, float b, float a)
{
	BlendFactor[0] = r;
	BlendFactor[1] = g;
	BlendFactor[2] = b;
	BlendFactor[3] = a;
}

void CBlendState::SetSampleMask(UINT SampleMask)
{
	this->SampleMask = SampleMask;
}

bool CBlendState::CreateState(bool AlphaToCoverageEnable, bool IndependentBlendEnable)
{
	if (TargetDescs.empty())
	{
		return false;
	}

	D3D11_BLEND_DESC Desc =
	{
		.AlphaToCoverageEnable = AlphaToCoverageEnable,
		.IndependentBlendEnable = IndependentBlendEnable
	};

	size_t Size = TargetDescs.size();

	memcpy(Desc.RenderTarget, &TargetDescs[0], sizeof(D3D11_RENDER_TARGET_BLEND_DESC) * Size);

	// Fill the remaining parts with default values after copying.
	for (auto& RenderTarget : Desc.RenderTarget | std::views::drop(Size))
	{
		RenderTarget.BlendEnable = false;
		RenderTarget.SrcBlend = D3D11_BLEND_ONE;
		RenderTarget.DestBlend = D3D11_BLEND_ZERO;
		RenderTarget.BlendOp = D3D11_BLEND_OP_ADD;
		RenderTarget.SrcBlendAlpha = D3D11_BLEND_ONE;
		RenderTarget.DestBlendAlpha = D3D11_BLEND_ZERO;
		RenderTarget.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		RenderTarget.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBlendState(&Desc, (ID3D11BlendState**)&State)))
	{
		return false;
	}

	return true;
}

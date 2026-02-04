#include "CRenderTarget.h"

#include "../../CDevice.h"

CRenderTarget::CRenderTarget()
{
}

CRenderTarget::~CRenderTarget()
{
	SAFE_RELEASE(DepthTexture);
	SAFE_RELEASE(Depth);
	SAFE_RELEASE(PrevDepth);

	SAFE_RELEASE(Texture);
	SAFE_RELEASE(Target);
	SAFE_RELEASE(PrevTarget);
}

std::shared_ptr<CRenderTarget> CRenderTarget::Create(const std::string& Key, UINT Width, UINT Height,
	DXGI_FORMAT Format, bool bEnableDepth)
{
	static std::shared_ptr<CRenderTarget> Target;
	Target.reset(new CRenderTarget);

	Target->SetKey(Key);

	if (!Target->Init(Width, Height, Format, bEnableDepth))
	{
		Target.reset();
		return {};
	}

	return Target;
}

bool CRenderTarget::Init(UINT Width, UINT Height, DXGI_FORMAT Format, bool bEnableDepth)
{
	auto* Info = new FTextureInfo;

	Info->Width = Width;
	Info->Height = Height;

	D3D11_TEXTURE2D_DESC Desc{};
	Desc.Width = Width;
	Desc.Height = Height;
	Desc.MipLevels = 1;
	Desc.ArraySize = 1;
	Desc.Format = Format;
	Desc.SampleDesc.Count = 1;
	Desc.Usage = D3D11_USAGE_DEFAULT;
	Desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateTexture2D(&Desc, nullptr, &Texture)))
	{
		SAFE_DELETE(Info);
		return false;
	}

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateRenderTargetView(Texture, nullptr, &Target)))
	{
		SAFE_DELETE(Info);
		return false;
	}

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateShaderResourceView(Texture, nullptr, &Info->SRV)))
	{
		SAFE_DELETE(Info);
		return false;
	}

	VP.Width = (float)Width;
	VP.Height = (float)Height;
	VP.MaxDepth = 1.f;

	TextureInfos.push_back(Info);

	if (bEnableDepth)
	{
		D3D11_TEXTURE2D_DESC DepthDesc{};
		DepthDesc.Width = Width;
		DepthDesc.Height = Height;
		DepthDesc.MipLevels = 1;
		DepthDesc.ArraySize = 1;
		DepthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		DepthDesc.SampleDesc.Count = 1;
		DepthDesc.Usage = D3D11_USAGE_DEFAULT;
		DepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		if (FAILED(CDevice::GetInst()->GetDevice()->CreateTexture2D(&DepthDesc, nullptr, &DepthTexture)))
		{
			return false;
		}

		if (FAILED(CDevice::GetInst()->GetDevice()->CreateDepthStencilView(DepthTexture, nullptr, &Depth)))
		{
			return false;
		}
	}

	return true;
}

void CRenderTarget::SetTarget()
{
	auto Context = CDevice::GetInst()->GetContext();

	ID3D11ShaderResourceView* NullSRV = nullptr;

	Context->PSSetShaderResources(0, 1, &NullSRV);

	Context->OMGetRenderTargets(1, &PrevTarget, &PrevDepth);
	Context->RSGetViewports(&PrevVPCount, &PrevVP);

	Context->RSSetViewports(1, &VP);

	ID3D11DepthStencilView* Depth = this->Depth;

	if (!this->Depth)
	{
		Depth = PrevDepth;
	}

	Context->OMSetRenderTargets(1, &Target, Depth);

	Context->ClearRenderTargetView(Target, ClearColor);

	if (this->Depth)
	{
		Context->ClearDepthStencilView(Depth, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	}
}

void CRenderTarget::ResetTarget()
{
	auto Context = CDevice::GetInst()->GetContext();

	Context->RSSetViewports(1, &PrevVP);
	Context->OMSetRenderTargets(1, &PrevTarget, PrevDepth);

	SAFE_RELEASE(PrevTarget);
	SAFE_RELEASE(PrevDepth);
}

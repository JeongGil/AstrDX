#include "CTextureManager.h"

#include <ranges>

#include "../../CDevice.h"

CTextureManager::~CTextureManager()
{
	for (auto& Sampler : Samplers | std::views::values)
	{
		SAFE_RELEASE(Sampler);
	}
}

bool CTextureManager::Init()
{
	CreateSampler("Point", D3D11_FILTER_MIN_MAG_MIP_POINT);
	CreateSampler("Linear", D3D11_FILTER_MIN_MAG_MIP_LINEAR);

	SetSampler("Point", 0, EShaderBufferType::ALL);
	SetSampler("Point", 0, EShaderBufferType::ALL);

	return true;
}

void CTextureManager::ReleaseAsset(const std::string& Key)
{
	auto It = Textures.find(Key);
	if (It != Textures.end())
	{
		if (It->second.use_count() == 1)
		{
			Textures.erase(It);
		}
	}
}

std::weak_ptr<CTexture> CTextureManager::FindTexture(const std::string& Key)
{
	auto It = Textures.find(Key);
	if (It == Textures.end())
	{
		return std::weak_ptr<CTexture>();
	}
	else
	{
		return It->second;
	}
}

bool CTextureManager::LoadTexture(const std::string& Key, const TCHAR* FileName, const std::string& PathName)
{
	if (Textures.contains(Key))
	{
		return true;
	}

	std::shared_ptr<CTexture> NewTex(new CTexture);

	NewTex->Name = Key;

	if (!NewTex->LoadTexture(FileName, PathName))
	{
		return false;
	}

	Textures.emplace(Key, NewTex);

	return true;
}

bool CTextureManager::LoadTextureFullPath(const std::string& Key, const TCHAR* FullPath, const std::string& PathName)
{
	if (Textures.contains(Key))
	{
		return true;
	}

	std::shared_ptr<CTexture> NewTex(new CTexture);

	NewTex->Name = Key;

	if (!NewTex->LoadTextureFullPath(FullPath))
	{
		return false;
	}

	Textures.emplace(Key, NewTex);

	return true;
}

bool CTextureManager::LoadTextures(const std::string& Key, const std::vector<const TCHAR*>& FileNames,
	const std::string& PathName)
{
	if (Textures.contains(Key))
	{
		return true;
	}

	std::shared_ptr<CTexture> NewTex(new CTexture);

	NewTex->Name = Key;

	if (!NewTex->LoadTextures(FileNames, PathName))
	{
		return false;
	}

	Textures.emplace(Key, NewTex);

	return true;
}

bool CTextureManager::LoadTexturesFullPath(const std::string& Key, const std::vector<const TCHAR*>& FullPaths,
	const std::string& PathName)
{
	if (Textures.contains(Key))
	{
		return true;
	}

	std::shared_ptr<CTexture> NewTex(new CTexture);

	NewTex->Name = Key;

	if (!NewTex->LoadTexturesFullPath(FullPaths))
	{
		return false;
	}

	Textures.emplace(Key, NewTex);

	return true;
}

bool CTextureManager::CreateSampler(const std::string& Key, D3D11_FILTER Filter, D3D11_TEXTURE_ADDRESS_MODE u,
	D3D11_TEXTURE_ADDRESS_MODE v, D3D11_TEXTURE_ADDRESS_MODE w, UINT MaxAnisotropy, D3D11_COMPARISON_FUNC Func)
{
	if (Samplers.contains(Key))
	{
		return false;
	}

	D3D11_SAMPLER_DESC Desc =
	{
		.Filter = Filter,
		.AddressU = u,
		.AddressV = v,
		.AddressW = w,
		.MipLODBias = 0.f,
		.MaxAnisotropy = MaxAnisotropy,
		.ComparisonFunc = Func,
		.BorderColor =
		{
			0.f,
			0.f,
			0.f,
			1.f
		},
		.MinLOD = 0.f,
		.MaxLOD = D3D11_FLOAT32_MAX,
	};

	ID3D11SamplerState* Sampler = nullptr;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateSamplerState(&Desc, &Sampler)))
	{
		return false;
	}

	Samplers.emplace(Key, Sampler);

	return true;
}

ID3D11SamplerState* CTextureManager::FindSampler(const std::string& Key)
{
	auto It = Samplers.find(Key);
	if (It == Samplers.end())
	{
		return nullptr;
	}
	else
	{
		return It->second;
	}
}

void CTextureManager::SetSampler(const std::string& Key, int Register, unsigned int ShaderBufferType)
{
	auto Sampler = FindSampler(Key);
	if (!Sampler)
	{
		return;
	}

	if ((ShaderBufferType & EShaderBufferType::Vertex) != 0)
	{
		CDevice::GetInst()->GetContext()->VSSetSamplers(Register, 1, &Sampler);
	}

	if ((ShaderBufferType & EShaderBufferType::Pixel) != 0)
	{
		CDevice::GetInst()->GetContext()->PSSetSamplers(Register, 1, &Sampler);
	}

	if ((ShaderBufferType & EShaderBufferType::Domain) != 0)
	{
		CDevice::GetInst()->GetContext()->DSSetSamplers(Register, 1, &Sampler);
	}

	if ((ShaderBufferType & EShaderBufferType::Hull) != 0)
	{
		CDevice::GetInst()->GetContext()->HSSetSamplers(Register, 1, &Sampler);
	}

	if ((ShaderBufferType & EShaderBufferType::Geometry) != 0)
	{
		CDevice::GetInst()->GetContext()->GSSetSamplers(Register, 1, &Sampler);
	}

	if ((ShaderBufferType & EShaderBufferType::Compute) != 0)
	{
		CDevice::GetInst()->GetContext()->CSSetSamplers(Register, 1, &Sampler);
	}
}

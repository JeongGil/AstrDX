#pragma once

#include "../../EngineInfo.h"
#include "CTexture.h"
#include "../Shader/CBufferContainer.h"

class CTextureManager
{
	friend class CAssetManager;

private:
	CTextureManager() = default;
public:
	~CTextureManager();

	bool Init();

	void ReleaseAsset(const std::string& Key);

	std::weak_ptr<CTexture> FindTexture(const std::string& Key);

	bool LoadTexture(const std::string& Key, const TCHAR* FileName, const std::string& PathName = "Texture");
	bool LoadTextureFullPath(const std::string& Key, const TCHAR* FullPath, const std::string& PathName = "Texture");
	bool LoadTextures(const std::string& Key, const std::vector<const TCHAR*>& FileNames, const std::string& PathName = "Texture");
	bool LoadTexturesFullPath(const std::string& Key, const std::vector<const TCHAR*>& FullPaths, const std::string& PathName = "Texture");

	bool CreateSampler(const std::string& Key, D3D11_FILTER Filter,
		D3D11_TEXTURE_ADDRESS_MODE u = D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_MODE v = D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_MODE w = D3D11_TEXTURE_ADDRESS_WRAP,
		UINT MaxAnisotropy = 0,
		D3D11_COMPARISON_FUNC Func = D3D11_COMPARISON_NEVER);

	ID3D11SamplerState* FindSampler(const std::string& Key);

	void SetSampler(const std::string& Key, int Register, unsigned int ShaderBufferType = EShaderBufferType::Pixel);

private:
	std::unordered_map<std::string, std::shared_ptr<CTexture>> Textures;
	std::unordered_map<std::string, ID3D11SamplerState*> Samplers;
};


#pragma once
#include "../CAsset.h"

#include "../Shader/CBufferContainer.h"

class CCBufferMaterial;
class CTexture;
class CRenderState;

struct FMaterialTextureInfo;

class CMaterial :
    public CAsset
{
	friend class CMaterialManager;
	friend class CMesh;

public:
	std::weak_ptr<CRenderState> GetBlendState() const
	{
		return BlendState;
	}

	std::weak_ptr<CTexture> GetTexture(int Index = 0) const;
	void SetBlendState(const std::string& Key);
	void SetBlendState(const std::weak_ptr<CRenderState>& State);

	FColor GetBaseColor() const
	{
		return BaseColor;
	}

	void SetBaseColor(float r, float g, float b, float a);
	void SetBaseColor(int r, int g, int b, int a);
	void SetBaseColor(const FColor& Color);
	void SetOpacity(float Opacity);

	std::weak_ptr<FMaterialTextureInfo> AddTexture(const std::weak_ptr<CTexture>& Texture, int Register,
	                                               int ShaderBufferType = EShaderBufferType::Pixel, int Index = 0);

	std::weak_ptr<FMaterialTextureInfo> AddTexture(const std::string& Key, int Register,
	                                               int ShaderBufferType = EShaderBufferType::Pixel, int Index = 0);
	std::weak_ptr<FMaterialTextureInfo> AddTexture(const std::string& Key, const TCHAR* FileName,
	                                               const std::string& PathName = "Texture", int Register = 0,
	                                               int ShaderBufferType = EShaderBufferType::Pixel, int Index = 0);
	std::weak_ptr<FMaterialTextureInfo> AddTextureFullPath(const std::string& Key, const TCHAR* FullPath,
	                                                       int Register = 0,
	                                                       int ShaderBufferType = EShaderBufferType::Pixel,
	                                                       int Index = 0);

	std::weak_ptr<FMaterialTextureInfo> AddTextures(const std::string& Key, const std::vector<const TCHAR*>& FileNames,
	                                                const std::string& PathName = "Texture", int Register = 0,
	                                                int ShaderBufferType = EShaderBufferType::Pixel, int Index = 0);
	std::weak_ptr<FMaterialTextureInfo> AddTexturesFullPath(const std::string& Key,
	                                                        const std::vector<const TCHAR*>& FullPaths, int Register = 0,
	                                                        int ShaderBufferType = EShaderBufferType::Pixel,
	                                                        int Index = 0);

	std::weak_ptr<FMaterialTextureInfo> AddTextureArray(const std::string& Key, const std::vector<const TCHAR*>& FileNames,
	                                                    const std::string& PathName = "Texture", int Register = 0,
	                                                    int ShaderBufferType = EShaderBufferType::Pixel, int Index = 0);
	std::weak_ptr<FMaterialTextureInfo> AddTextureArrayFullPath(const std::string& Key,
	                                                            const std::vector<const TCHAR*>& FullPaths, int Register = 0,
	                                                            int ShaderBufferType = EShaderBufferType::Pixel,
	                                                            int Index = 0);

	bool SetTexture(int TextureIndex, const std::weak_ptr<CTexture>& Texture);

protected:
	FColor BaseColor = FColor::White;
	float Opacity = 1.f;

	std::weak_ptr<CRenderState> BlendState;

	std::shared_ptr<CCBufferMaterial> MaterialCBuffer;

	std::vector<std::shared_ptr<FMaterialTextureInfo>> TextureInfos;

public:
	bool Init();
	void UpdateConstantBuffer();
	void UpdateConstantBuffer(int TextureIndex);
	void Reset();
	CMaterial* Clone() const;

protected:
	CMaterial();

	CMaterial(const CMaterial& other);

	CMaterial(CMaterial&& other) noexcept;

	CMaterial& operator=(const CMaterial& other);

	CMaterial& operator=(CMaterial&& other) noexcept;

public:
	~CMaterial() override = default;
};

struct FMaterialTextureInfo
{
	std::string Key;
	std::weak_ptr<CTexture> Texture;
	int Register = 0;
	int ShaderBufferType = EShaderBufferType::Pixel;
	int Index = 0;
};


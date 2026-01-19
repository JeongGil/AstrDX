#include "CMaterial.h"

#include "../CAssetManager.h"
#include "../../Render/CRenderManager.h"
#include "../../Render/CRenderState.h"
#include "../Shader/CCBufferMaterial.h"
#include "../Texture/CTexture.h"
#include "../Texture/CTextureManager.h"

void CMaterial::SetBlendState(const std::string& Key)
{
	BlendState = CRenderManager::GetInst()->FindRenderState(Key);
}

void CMaterial::SetBlendState(const std::weak_ptr<CRenderState>& State)
{
	BlendState = State;
}

void CMaterial::SetBaseColor(float r, float g, float b, float a)
{
	BaseColor = FColor(r, g, b, a);
	MaterialCBuffer->SetBaseColor(BaseColor);
}

void CMaterial::SetBaseColor(int r, int g, int b, int a)
{
	BaseColor = FColor(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
	MaterialCBuffer->SetBaseColor(BaseColor);
}

void CMaterial::SetBaseColor(const FColor& Color)
{
	BaseColor = Color;
	MaterialCBuffer->SetBaseColor(BaseColor);
}

void CMaterial::SetOpacity(float Opacity)
{
	this->Opacity = Opacity;
	MaterialCBuffer->SetOpacity(Opacity);
}

std::weak_ptr<FMaterialTextureInfo> CMaterial::AddTexture(const std::weak_ptr<CTexture>& Texture, int Register,
                                                          int ShaderBufferType, int Index)
{
	auto Shared = Texture.lock();
	std::shared_ptr<FMaterialTextureInfo> TexInfo(new FMaterialTextureInfo
		{
			.Name = Shared->GetName(),
			.Texture = Texture,
			.Register = Register,
			.ShaderBufferType = ShaderBufferType,
			.Index = Index
		});

	TextureInfos.push_back(TexInfo);
	return TexInfo;
}

std::weak_ptr<FMaterialTextureInfo> CMaterial::AddTexture(const std::string& Key, int Register, int ShaderBufferType,
                                                          int Index)
{
	auto TexMgr = CAssetManager::GetInst()->GetTextureManager().lock();
	std::shared_ptr<FMaterialTextureInfo> TexInfo(new FMaterialTextureInfo
		{
			.Name = Key,
			.Texture = TexMgr->FindTexture(Key),
			.Register = Register,
			.ShaderBufferType = ShaderBufferType,
			.Index = Index
		});

	TextureInfos.push_back(TexInfo);
	return TexInfo;
}

std::weak_ptr<FMaterialTextureInfo> CMaterial::AddTexture(const std::string& Key, const TCHAR* FileName,
                                                          const std::string& PathName, int Register,
                                                          int ShaderBufferType, int Index)
{
	auto TexMgr = CAssetManager::GetInst()->GetTextureManager().lock();

	TexMgr->LoadTexture(Key, FileName, PathName);

	std::shared_ptr<FMaterialTextureInfo> TexInfo(new FMaterialTextureInfo
		{
			.Name = Key,
			.Texture = TexMgr->FindTexture(Key),
			.Register = Register,
			.ShaderBufferType = ShaderBufferType,
			.Index = Index
		});

	TextureInfos.push_back(TexInfo);
	return TexInfo;
}

std::weak_ptr<FMaterialTextureInfo> CMaterial::AddTextureFullPath(const std::string& Key, const TCHAR* FullPath, int Register, int ShaderBufferType,
	int Index)
{
	auto TexMgr = CAssetManager::GetInst()->GetTextureManager().lock();

	TexMgr->LoadTextureFullPath(Key, FullPath);

	std::shared_ptr<FMaterialTextureInfo> TexInfo(new FMaterialTextureInfo
		{
			.Name = Key,
			.Texture = TexMgr->FindTexture(Key),
			.Register = Register,
			.ShaderBufferType = ShaderBufferType,
			.Index = Index
		});

	TextureInfos.push_back(TexInfo);
	return TexInfo;
}

std::weak_ptr<FMaterialTextureInfo> CMaterial::AddTextures(const std::string& Key, std::vector<const TCHAR*>& FileNames,
                                                           const std::string& PathName,
                                                           int Register, int ShaderBufferType, int Index)
{
	auto TexMgr = CAssetManager::GetInst()->GetTextureManager().lock();

	TexMgr->LoadTextures(Key, FileNames, PathName);

	std::shared_ptr<FMaterialTextureInfo> TexInfo(new FMaterialTextureInfo
		{
			.Name = Key,
			.Texture = TexMgr->FindTexture(Key),
			.Register = Register,
			.ShaderBufferType = ShaderBufferType,
			.Index = Index
		});

	TextureInfos.push_back(TexInfo);
	return TexInfo;
}

std::weak_ptr<FMaterialTextureInfo> CMaterial::AddTexturesFullPath(const std::string& Key,
                                                                   std::vector<const TCHAR*>& FullPaths, int Register,
                                                                   int ShaderBufferType, int Index)
{
	auto TexMgr = CAssetManager::GetInst()->GetTextureManager().lock();

	TexMgr->LoadTexturesFullPath(Key, FullPaths);

	std::shared_ptr<FMaterialTextureInfo> TexInfo(new FMaterialTextureInfo
		{
			.Name = Key,
			.Texture = TexMgr->FindTexture(Key),
			.Register = Register,
			.ShaderBufferType = ShaderBufferType,
			.Index = Index
		});

	TextureInfos.push_back(TexInfo);
	return TexInfo;
}

bool CMaterial::SetTexture(int TextureIndex, const std::weak_ptr<CTexture>& Texture)
{
	if (TextureIndex >= TextureInfos.size())
	{
		if (auto Tex = Texture.lock())
		{
			TextureInfos.emplace_back(new FMaterialTextureInfo
			   {
				   .Name = Tex->GetName(),
				   .Register = 0,
				   .ShaderBufferType = EShaderBufferType::Pixel,
				   .Index = 0,
			   });
		}
	}

	TextureInfos[TextureIndex]->Texture = Texture;

	return true;
}

bool CMaterial::SetTextureIndex(int TextureIndex)
{
	return true;
}

void CMaterial::UpdateConstantBuffer(int TextureIndex)
{
	for (auto& TextureInfo : TextureInfos)
	{
		if (auto Texture = TextureInfo->Texture.lock())
		{
			Texture->SetShader(TextureInfo->Register, TextureInfo->ShaderBufferType, TextureIndex);
		}
	}

	MaterialCBuffer->UpdateBuffer();

	if (auto BlendState = this->BlendState.lock())
	{
		BlendState->SetState();
	}
}

bool CMaterial::Init()
{
	MaterialCBuffer.reset(new CCBufferMaterial);
	MaterialCBuffer->Init();
	MaterialCBuffer->SetBaseColor(BaseColor);
	MaterialCBuffer->SetOpacity(Opacity);

	return true;
}

void CMaterial::UpdateConstantBuffer()
{
	for (auto& TextureInfo : TextureInfos)
	{
		if (auto Texture = TextureInfo->Texture.lock())
		{
			Texture->SetShader(TextureInfo->Register, TextureInfo->ShaderBufferType, TextureInfo->Index);
		}
	}

	MaterialCBuffer->UpdateBuffer();

	if (auto BlendState = this->BlendState.lock())
	{
		BlendState->SetState();
	}
}

void CMaterial::Reset()
{
	if (auto BlendState = this->BlendState.lock())
	{
		BlendState->ResetState();
	}
}

CMaterial* CMaterial::Clone() const
{
	return new CMaterial(*this);
}

CMaterial::CMaterial()
{
	AssetType = EAssetType::Material;
}

CMaterial::CMaterial(const CMaterial& other) : CAsset(other),
                                               BaseColor(other.BaseColor),
                                               Opacity(other.Opacity)
//MaterialCBuffer(other.MaterialCBuffer)
{
	MaterialCBuffer.reset(new CCBufferMaterial);
	MaterialCBuffer->Init();
	MaterialCBuffer->SetBaseColor(BaseColor);
	MaterialCBuffer->SetOpacity(Opacity);
}

CMaterial::CMaterial(CMaterial&& other) noexcept : CAsset(std::move(other)),
BaseColor(std::move(other.BaseColor)),
Opacity(other.Opacity),
MaterialCBuffer(std::move(other.MaterialCBuffer))
{
}

CMaterial& CMaterial::operator=(const CMaterial& other)
{
	if (this == &other)
		return *this;
	CAsset::operator =(other);
	BaseColor = other.BaseColor;
	Opacity = other.Opacity;
	//MaterialCBuffer = other.MaterialCBuffer;
	MaterialCBuffer.reset(new CCBufferMaterial);
	MaterialCBuffer->Init();
	MaterialCBuffer->SetBaseColor(BaseColor);
	MaterialCBuffer->SetOpacity(Opacity);
	return *this;
}

CMaterial& CMaterial::operator=(CMaterial&& other) noexcept
{
	if (this == &other)
		return *this;
	CAsset::operator =(std::move(other));
	BaseColor = std::move(other.BaseColor);
	Opacity = other.Opacity;
	MaterialCBuffer = std::move(other.MaterialCBuffer);
	return *this;
}

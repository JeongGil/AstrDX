#include "CAssetManager.h"

#include "CPathManager.h"
#include "Animation2D/CAnimation2DManager.h"
#include "Mesh/CMeshManager.h"
#include "Shader/CShaderManager.h"
#include "Sound/CSoundManager.h"
#include "Texture/CTextureManager.h"

bool CAssetManager::Init()
{
	if (!CPathManager::Init())
	{
		return false;
	}

	ShaderManager.reset(new CShaderManager);
	if (!ShaderManager->Init())
	{
		return false;
	}

	TextureManager.reset(new CTextureManager);
	if (!TextureManager->Init())
	{
		return false;
	}

	MeshManager.reset(new CMeshManager);
	if (!MeshManager->Init())
	{
		return false;
	}

	Animation2DManager.reset(new CAnimation2DManager);
	if (!Animation2DManager->Init())
	{
		return false;
	}

	SoundManager.reset(new CSoundManager);
	if (!SoundManager->Init())
	{
		return false;
	}

	return true;
}

void CAssetManager::ReleaseAsset(const std::string& Key, EAssetType AssetType)
{
	switch (AssetType)
	{
		case EAssetType::Mesh:
			MeshManager->ReleaseAsset(Key);
			break;
		case EAssetType::Shader:
			ShaderManager->ReleaseShader(Key);
			break;
		case EAssetType::ConstantBuffer:
			ShaderManager->ReleaseCBuffer(Key);
			break;
		case EAssetType::Material:
			break;
		case EAssetType::Texture:
			TextureManager->ReleaseAsset(Key);
			break;
		case EAssetType::Animation2D:
			Animation2DManager->ReleaseAsset(Key);
			break;
		case EAssetType::Sound:
			SoundManager->ReleaseAsset(Key);
			break;
		case EAssetType::Font:
			break;
		case EAssetType::None:
		default:
			break;
	}
}

CAssetManager::CAssetManager()
{
}

CAssetManager::~CAssetManager()
{
	CPathManager::Clear();
}

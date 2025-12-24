#include "CAssetManager.h"

#include "CPathManager.h"
#include "Mesh/CMeshManager.h"
#include "Shader/CShaderManager.h"
#include "Texture/CTextureManager.h"
#include "Animation2D/CAnimation2DManager.h"

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

	return true;
}

CAssetManager::CAssetManager()
{
}

CAssetManager::~CAssetManager()
{
	CPathManager::Clear();
}

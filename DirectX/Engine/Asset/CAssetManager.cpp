#include "CAssetManager.h"

#include "CPathManager.h"
#include "Mesh/CMeshManager.h"
#include "Shader/CShaderManager.h"

bool CAssetManager::Init()
{
	if (!CPathManager::Init())
	{
		return false;
	}

	MeshManager.reset(new CMeshManager);
	if (!MeshManager->Init())
	{
		return false;
	}

	ShaderManager.reset(new CShaderManager);
	if (!ShaderManager->Init())
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

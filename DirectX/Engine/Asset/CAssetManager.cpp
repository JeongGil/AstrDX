#include "CAssetManager.h"

#include "Mesh/CMeshManager.h"

bool CAssetManager::Init()
{
	meshManager.reset(new CMeshManager);
	if (!meshManager->Init())
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
}

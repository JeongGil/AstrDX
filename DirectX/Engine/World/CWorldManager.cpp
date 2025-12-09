#include "CWorldManager.h"

#include "CWorld.h"

bool CWorldManager::Init()
{
	world = std::make_shared<CWorld>();
	if (!world || !world->Init())
	{
		return false;
	}

	return true;
}

void CWorldManager::Update(const float deltaTime)
{
	world->Update(deltaTime);
}

void CWorldManager::Render()
{
	world->Render();
}

CWorldManager::CWorldManager()
{
}

CWorldManager::~CWorldManager()
{
}

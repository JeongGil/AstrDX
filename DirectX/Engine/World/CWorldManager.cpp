#include "CWorldManager.h"

#include "CWorld.h"

bool CWorldManager::Init()
{
	World = std::make_shared<CWorld>();
	if (!World || !World->Init())
	{
		return false;
	}

	return true;
}

void CWorldManager::Update(const float DeltaTime)
{
	World->Update(DeltaTime);
}

void CWorldManager::PostUpdate(const float DeltaTime)
{
	World->PostUpdate(DeltaTime);
}

void CWorldManager::Render()
{
	World->Render();
}

CWorldManager::CWorldManager()
{
}

CWorldManager::~CWorldManager()
{
}

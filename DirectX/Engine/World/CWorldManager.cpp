#include "CWorldManager.h"

#include "CWorld.h"

void CWorldManager::InputActive()
{
	World->InputActive();
}

void CWorldManager::InputDeactive()
{
	World->InputDeactive();
}

bool CWorldManager::Init()
{
	World = std::make_shared<CWorld>();
	if (!World || !World->Init())
	{
		return false;
	}

	return true;
}

bool CWorldManager::Update(const float DeltaTime)
{
	World->Update(DeltaTime);

	return ChangeWorld();
}

bool CWorldManager::PostUpdate(const float DeltaTime)
{
	World->PostUpdate(DeltaTime);

	return ChangeWorld();
}

void CWorldManager::Render()
{
	World->Render();
}

void CWorldManager::RenderUI()
{
	World->RenderUI();
}

void CWorldManager::PostRender()
{
	World->PostRender();
}

bool CWorldManager::ChangeWorld()
{
	if (NextWorld)
	{
		World = std::move(NextWorld);

		return true;
	}

	return false;
}

CWorldManager::CWorldManager()
{
}

CWorldManager::~CWorldManager()
{
}

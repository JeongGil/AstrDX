#include "CWorld.h"

bool CWorld::Init()
{
	CameraManager.reset(new CCameraManager);
	if (!CameraManager->Init())
	{
		return false;
	}

	WorldAssetManager.reset(new CWorldAssetManager);
	if (!WorldAssetManager->Init())
	{
		return false;
	}

	Input.reset(new CInput);
	if (!Input->Init())
	{
		return false;
	}

	Objects.reserve(10000);

	return true;
}

void CWorld::Update(const float DeltaTime)
{
	Input->Update(DeltaTime);

	auto Curr = Objects.begin();
	const auto End = Objects.end();
	while (Curr != End)
	{
		if (!Curr->second->GetAlive())
		{
			Curr = Objects.erase(Curr);
			continue;
		}

		Curr->second->Update(DeltaTime);
		++Curr;
	}

	CameraManager->Update(DeltaTime);

	WorldAssetManager->Update(DeltaTime);
}

void CWorld::PostUpdate(const float DeltaTime)
{
	auto Curr = Objects.begin();
	const auto End = Objects.end();
	while (Curr != End)
	{
		if (!Curr->second->GetAlive())
		{
			Curr = Objects.erase(Curr);
			continue;
		}

		Curr->second->PostUpdate(DeltaTime);
		++Curr;
	}
}

void CWorld::Render()
{
	auto Curr = Objects.begin();
	const auto End = Objects.end();
	while (Curr != End)
	{
		if (!Curr->second->GetAlive())
		{
			Curr = Objects.erase(Curr);
			continue;
		}

		Curr->second->Render();
		++Curr;
	}
}

CWorld::CWorld()
{
}

CWorld::~CWorld()
{
}

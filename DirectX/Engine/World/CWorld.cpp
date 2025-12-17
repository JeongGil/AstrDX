#include "CWorld.h"

bool CWorld::Init()
{
	CameraManager.reset(new CCameraManager);
	if (!CameraManager->Init())
	{
		return false;
	}

	return true;
}

void CWorld::Update(const float DeltaTime)
{
	auto Curr = Objects.begin();
	const auto End = Objects.end();
	while (Curr != End)
	{
		if (!(*Curr)->GetAlive())
		{
			Curr = Objects.erase(Curr);
			continue;
		}

		(*Curr)->Update(DeltaTime);
		++Curr;
	}

	CameraManager->Update(DeltaTime);
}

void CWorld::PostUpdate(const float DeltaTime)
{
	auto Curr = Objects.begin();
	const auto End = Objects.end();
	while (Curr != End)
	{
		if (!(*Curr)->GetAlive())
		{
			Curr = Objects.erase(Curr);
			continue;
		}

		(*Curr)->PostUpdate(DeltaTime);
		++Curr;
	}
}

void CWorld::Render()
{
	auto Curr = Objects.begin();
	const auto End = Objects.end();
	while (Curr != End)
	{
		if (!(*Curr)->GetAlive())
		{
			Curr = Objects.erase(Curr);
			continue;
		}

		(*Curr)->Render();
		++Curr;
	}
}

CWorld::CWorld()
{
}

CWorld::~CWorld()
{
}

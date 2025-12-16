#include "CWorld.h"

bool CWorld::Init()
{
	return true;
}

void CWorld::Update(float DeltaTime)
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

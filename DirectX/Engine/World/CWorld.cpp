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

	Collision.reset(new CWorldCollision);
	if (!Collision->Init())
	{
		return false;
	}

	Objects.reserve(10000);
	StartObjects.reserve(200);

	return true;
}

void CWorld::Update(const float DeltaTime)
{
	Begin();

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
	Begin();

	auto It = Objects.begin();
	while (It != Objects.end())
	{
		if (!It->second->GetAlive())
		{
			It = Objects.erase(It);
			continue;
		}

		It->second->PostUpdate(DeltaTime);
		++It;
	}

	Collision->Update(DeltaTime);

	It = Objects.begin();
	while (It != Objects.end())
	{
		auto Obj = It->second;
		if (Obj.use_count() == 0)
		{
			It = Objects.erase(It);
			continue;
		}

		if (!Obj->GetAlive())
		{
			It = Objects.erase(It);
			continue;
		}

		if (!Obj->GetEnable())
		{
			It = Objects.erase(It);
			continue;
		}

		Obj->UpdateTransform();
		++It;
	}
}

void CWorld::Render()
{
	Begin();

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

void CWorld::Begin()
{
	for (const auto& WeakObject : StartObjects)
	{
		if (auto Object = WeakObject.lock())
		{
			Object->Begin();
		}
	}

	StartObjects.clear();
}

CWorld::CWorld()
{
}

CWorld::~CWorld()
{
}

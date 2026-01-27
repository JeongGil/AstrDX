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
	Input->World = shared_from_this();
	if (!Input->Init())
	{
		return false;
	}

	Collision.reset(new CWorldCollision);
	Collision->SetWorld(shared_from_this());
	if (!Collision->Init())
	{
		return false;
	}

	UIManager.reset(new CWorldUIManager);
	UIManager->World = shared_from_this();
	if (!UIManager->Init())
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

	UIManager->Update(DeltaTime);
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

#ifdef _DEBUG

	Collision->Render();

#endif

	Collision->ReturnNodePool();
}

void CWorld::PostRender()
{
	std::erase_if(Objects, [](auto& Pair)
		{
			if (!Pair.second->GetAlive())
			{
				// TODO: OnDestroy is not implemented in the component (widget), so it is commented out
				// Pair.second->OnDestroy();
				return true;
			}
			return false;
		});

	for (auto& Object : Objects | std::views::values)
	{
		Object->PostRender();
	}
}

void CWorld::RenderUI()
{
	UIManager->Render();
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

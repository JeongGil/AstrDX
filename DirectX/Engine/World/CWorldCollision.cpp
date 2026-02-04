#include "CWorldCollision.h"

#include "../Component/CCollider.h"
#include "../Object/CGameObject.h"
#include "../Component/CCollision.h"
#include "CWorldUIManager.h"
#include "CWorld.h"
#include "CInput.h"

bool CWorldCollision::Init()
{
	QuadTree.reset(new CCollisionQuadTree);
	if (!QuadTree->Init())
	{
		return false;
	}

	QuadTree->SetWorld(World);

	return true;
}

void CWorldCollision::Update(const float DeltaTime)
{
	QuadTree->Update(DeltaTime);

	std::erase_if(Colliders, [&](const auto& WeakCollider)
		{
			auto Collider = WeakCollider.lock();
			if (!Collider || !Collider->GetAlive())
			{
				return true;
			}

			Collider->UpdateInfo();
			if (!Collider->GetEnable())
			{
				return false;
			}

			QuadTree->AddCollider(WeakCollider);

			return false;
		});

	auto World = this->World.lock();
	auto UIManager = World->GetUIManager().lock();

	FVector2 MousePos = World->GetInput().lock()->GetMousePos();

	if (!UIManager->CollideMouse(DeltaTime, MousePos))
	{
		FVector2 MouseWorldPos = World->GetInput().lock()->GetMouseWorldPos();
		QuadTree->CollideMouse(HoveredObject, DeltaTime, MouseWorldPos);
	}
	else if (auto HoveredObject = this->HoveredObject.lock())
	{
		FVector2 MouseWorldPos = World->GetInput().lock()->GetMouseWorldPos();
		FVector ConvertPos(MouseWorldPos.x, MouseWorldPos.y, 0.f);

		HoveredObject->CallOnCollisionMouseEnd(ConvertPos);

		HoveredObject = {};
	}

	QuadTree->Collide(DeltaTime);
}

void CWorldCollision::Render()
{
	QuadTree->Render();
}

void CWorldCollision::ReturnNodePool()
{
	QuadTree->ReturnNodePool();
}

void CWorldCollision::UpdateInfo()
{
	auto It = Colliders.begin();

	while (It != Colliders.end())
	{
		auto Collider = It->lock();
		if (!Collider)
		{
			It = Colliders.erase(It);
			continue;
		}

		if (!Collider->GetAlive())
		{
			It = Colliders.erase(It);
			continue;
		}

		Collider->UpdateInfo();
		++It;
	}
}

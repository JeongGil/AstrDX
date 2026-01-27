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
	//if (Interval > 0.f)
	//{
	//	Elapsed += DeltaTime;
	//
	//	if (Elapsed < Interval)
	//	{
	//		return;
	//	}
	//
	//	Elapsed -= Interval;
	//}

	QuadTree->Update(DeltaTime);

	auto SrcIt = Colliders.begin();
	while (SrcIt != Colliders.end())
	{
		if (SrcIt->expired())
		{
			SrcIt  = Colliders.erase(SrcIt);
			continue;
		}

		auto SrcCollider = SrcIt->lock();

		if (!SrcCollider->GetAlive())
		{
			SrcIt = Colliders.erase(SrcIt);
			continue;
		}

		if (!SrcCollider->GetEnable())
		{
			SrcCollider->UpdateInfo();
			++SrcIt;
			continue;
		}

		SrcCollider->UpdateInfo();

		QuadTree->AddCollider(*SrcIt);

		++SrcIt;
	}

	auto World = this->World.lock();
	auto UIManager = World->GetUIManager().lock();

	FVector2 MousePos = World->GetInput().lock()->GetMousePos();

	if (!UIManager->CollideMouse(DeltaTime, MousePos))
	{
		FVector2 MouseWorldPos = World->GetInput().lock()->GetMouseWorldPos();
		FVector ConvertPos(MouseWorldPos.x, MouseWorldPos.y, 0.f);

		HoveredObject.lock()->CallOnCollisionMouseEnd(ConvertPos);

		HoveredObject = {};
	}

	QuadTree->Collide(DeltaTime);

	//UpdateInfo();

	//auto SrcIt = Colliders.begin();
	//while (SrcIt != Colliders.end())
	//{
	//	auto SrcCollider = SrcIt->lock();
	//	if (!SrcCollider)
	//	{
	//		SrcIt = Colliders.erase(SrcIt);
	//		continue;
	//	}

	//	if (!SrcCollider->GetAlive())
	//	{
	//		SrcIt = Colliders.erase(SrcIt);
	//		continue;
	//	}

	//	if (!SrcCollider->GetEnable())
	//	{
	//		++SrcIt;
	//		continue;
	//	}

	//	auto SrcProfile = SrcCollider->GetCollisionProfile();
	//	if (!SrcProfile || !SrcProfile->bEnable)
	//	{
	//		++SrcIt;
	//		continue;
	//	}

	//	auto DestIt = std::next(SrcIt);
	//	while (DestIt != Colliders.end())
	//	{
	//		std::shared_ptr<CCollider> DestCollider = DestIt->lock();
	//		if (!DestCollider)
	//		{
	//			DestIt = Colliders.erase(DestIt);
	//			continue;
	//		}

	//		if (!DestCollider->GetAlive())
	//		{
	//			DestIt = Colliders.erase(DestIt);
	//			continue;
	//		}

	//		if (!DestCollider->GetEnable())
	//		{
	//			++DestIt;
	//			continue;
	//		}

	//		auto DestProfile = DestCollider->GetCollisionProfile();
	//		if (!DestProfile || !DestProfile->bEnable)
	//		{
	//			++DestIt;
	//			continue;
	//		}

	//		auto Src2Dest = SrcProfile->Interaction[DestProfile->Channel->Channel];
	//		auto Dest2Src = DestProfile->Interaction[SrcProfile->Channel->Channel];

	//		// Check interaction
	//		if (Src2Dest == ECollisionInteraction::Ignore
	//			|| Dest2Src == ECollisionInteraction::Ignore)
	//		{
	//			++DestIt;
	//			continue;
	//		}

	//		if (Src2Dest != Dest2Src)
	//		{
	//			++DestIt;
	//			continue;
	//		}

	//		FVector3 HitPoint, BlockMove;
	//		if (SrcCollider->Collide(HitPoint, DestCollider))
	//		{
	//			auto SrcObj = SrcCollider->GetOwner().lock();
	//			auto DestObj = DestCollider->GetOwner().lock();

	//			if (Src2Dest == ECollisionInteraction::Block)
	//			{
	//				auto SrcVelocity = SrcObj->GetVelocity();
	//				auto DestVelocity = DestObj->GetVelocity();

	//				auto SrcPos = SrcObj->GetWorldPosition();
	//				auto DestPos = DestObj->GetWorldPosition();

	//				//if (!SrcVelocity.IsZero() && DestVelocity.IsZero())
	//				//{
	//				//	SrcObj->AddWorldPosition((SrcVelocity / -2.f) * 1.01f);
	//				//	DestObj->AddWorldPosition((DestVelocity / -2.f) * 1.01f);
	//				//
	//				//	SrcCollider->UpdateInfo();
	//				//}
	//				//else if (!SrcVelocity.IsZero())
	//				//{
	//				//	SrcObj->AddWorldPosition(-(SrcVelocity * 1.01f));
	//				//
	//				//	SrcCollider->UpdateInfo();
	//				//}
	//				//else if (!DestVelocity.IsZero())
	//				//{
	//				//	DestObj->AddWorldPosition(-(DestVelocity * 1.01f));
	//				//
	//				//	DestCollider->UpdateInfo();
	//				//}
	//				//
	//				//SrcObj->ClearPhysics();
	//				//DestObj->ClearPhysics();

	//				FCollisionManifold Manifold;

	//				if (SrcCollider)

	//				SrcCollider->CallOnCollisionBlock(HitPoint, DestCollider);
	//				DestCollider->CallOnCollisionBlock(HitPoint, SrcCollider);
	//			}
	//			else
	//			{
	//				// OnCollisionBegin
	//				if (!SrcCollider->CheckCollidingObject(DestCollider.get()))
	//				{
	//					SrcCollider->CallOnCollisionBegin(HitPoint, DestCollider);
	//					DestCollider->CallOnCollisionBegin(HitPoint, SrcCollider);
	//				}
	//			}
	//		}
	//		else if (SrcCollider->CheckCollidingObject(DestCollider.get()))
	//		{
	//			// OnCollisionEnd
	//			if (SrcProfile->Interaction[DestProfile->Channel->Channel] == ECollisionInteraction::Overlap)
	//			{
	//				SrcCollider->CallOnCollisionEnd(DestCollider.get());
	//				DestCollider->CallOnCollisionEnd(SrcCollider.get());
	//			}
	//			else
	//			{
	//				
	//			}
	//		}

	//		++DestIt;
	//	}

	//	++SrcIt;
	//}
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

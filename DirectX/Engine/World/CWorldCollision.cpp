#include "CWorldCollision.h"

#include "../Component/CCollider.h"
#include "../Object/CGameObject.h"

bool CWorldCollision::Init()
{
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

	UpdateInfo();

	auto SrcIt = Colliders.begin();
	while (SrcIt != Colliders.end())
	{
		auto SrcCollider = SrcIt->lock();
		if (!SrcCollider)
		{
			SrcIt = Colliders.erase(SrcIt);
			continue;
		}

		if (!SrcCollider->GetAlive())
		{
			SrcIt = Colliders.erase(SrcIt);
			continue;
		}

		if (!SrcCollider->GetEnable())
		{
			++SrcIt;
			continue;
		}

		auto SrcProfile = SrcCollider->GetCollisionProfile();
		if (!SrcProfile || !SrcProfile->bEnable)
		{
			++SrcIt;
			continue;
		}

		auto DestIt = std::next(SrcIt);
		while (DestIt != Colliders.end())
		{
			std::shared_ptr<CCollider> DestCollider = DestIt->lock();
			if (!DestCollider)
			{
				DestIt = Colliders.erase(DestIt);
				continue;
			}

			if (!DestCollider->GetAlive())
			{
				DestIt = Colliders.erase(DestIt);
				continue;
			}

			if (!DestCollider->GetEnable())
			{
				++DestIt;
				continue;
			}

			auto DestProfile = DestCollider->GetCollisionProfile();
			if (!DestProfile || !DestProfile->bEnable)
			{
				++DestIt;
				continue;
			}

			auto Src2Dest = SrcProfile->Interaction[DestProfile->Channel->Channel];
			auto Dest2Src = DestProfile->Interaction[SrcProfile->Channel->Channel];

			// Check interaction
			if (Src2Dest == ECollisionInteraction::Ignore
				|| Dest2Src == ECollisionInteraction::Ignore)
			{
				++DestIt;
				continue;
			}

			if (Src2Dest != Dest2Src)
			{
				++DestIt;
				continue;
			}

			FVector3 HitPoint;
			if (SrcCollider->Collide(HitPoint, DestCollider))
			{
				auto SrcObj = SrcCollider->GetOwner().lock();
				auto DestObj = DestCollider->GetOwner().lock();

				if (Src2Dest == ECollisionInteraction::Block)
				{
					auto SrcVelocity = SrcObj->GetVelocity();
					auto DestVelocity = DestObj->GetVelocity();

					if (!SrcVelocity.IsZero() && DestVelocity.IsZero())
					{
						SrcObj->AddWorldPosition((SrcVelocity / -2.f) * 1.01f);
						DestObj->AddWorldPosition((DestVelocity / -2.f) * 1.01f);

						SrcCollider->UpdateInfo();
					}
					else if (!SrcVelocity.IsZero())
					{
						SrcObj->AddWorldPosition(-(SrcVelocity * 1.01f));

						SrcCollider->UpdateInfo();
					}
					else if (!DestVelocity.IsZero())
					{
						DestObj->AddWorldPosition(-(DestVelocity * 1.01f));

						DestCollider->UpdateInfo();
					}

					SrcObj->ClearPhysics();
					DestObj->ClearPhysics();

					SrcCollider->CallOnCollisionBlock(HitPoint, DestCollider);
					DestCollider->CallOnCollisionBlock(HitPoint, SrcCollider);
				}
				else
				{
					// OnCollisionBegin
					if (!SrcCollider->CheckCollidingObject(DestCollider.get()))
					{
						SrcCollider->CallOnCollisionBegin(HitPoint, DestCollider);
						DestCollider->CallOnCollisionBegin(HitPoint, SrcCollider);
					}
				}
			}
			else if (SrcCollider->CheckCollidingObject(DestCollider.get()))
			{
				// OnCollisionEnd
				if (SrcProfile->Interaction[DestProfile->Channel->Channel] == ECollisionInteraction::Overlap)
				{
					SrcCollider->CallOnCollisionEnd(DestCollider.get());
					DestCollider->CallOnCollisionEnd(SrcCollider.get());
				}
				else
				{
					
				}
			}

			++DestIt;
		}

		++SrcIt;
	}
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

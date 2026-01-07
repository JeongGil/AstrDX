#include "CWorldCollision.h"

#include "../Component/CCollider.h"

bool CWorldCollision::Init()
{
	return true;
}

void CWorldCollision::Update(const float DeltaTime)
{
	if (Interval > 0.f)
	{
		Elapsed += DeltaTime;

		if (Elapsed < Interval)
		{
			return;
		}

		Elapsed -= Interval;
	}

	auto It = Colliders.begin();
	while (It != Colliders.end())
	{
		auto SrcCollider = It->lock();
		if (!SrcCollider)
		{
			It = Colliders.erase(It);
			continue;
		}

		if (!SrcCollider->GetAlive())
		{
			It = Colliders.erase(It);
			continue;
		}

		if (!SrcCollider->GetEnable())
		{
			++It;
			continue;
		}

		auto SrcProfile = SrcCollider->GetCollisionProfile();
		if (!SrcProfile || !SrcProfile->bEnable)
		{
			++It;
			continue;
		}

		auto DestIt = std::next(It);
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

			// Check interaction
			if (SrcProfile->Interaction[DestProfile->Channel->Channel] == ECollisionInteraction::Ignore
				|| DestProfile->Interaction[SrcProfile->Channel->Channel] == ECollisionInteraction::Ignore)
			{
				++DestIt;
				continue;
			}

			FVector3 HitPoint;
			if (SrcCollider->Collide(HitPoint, DestCollider))
			{
				// OnCollisionBegin
				if (!SrcCollider->CheckCollidingObject(DestCollider.get()))
				{
					SrcCollider->CallOnCollisionBegin(HitPoint, DestCollider);
					DestCollider->CallOnCollisionBegin(HitPoint, SrcCollider);
				}
			}
			else
			{
				// OnCollisionEnd
				if (SrcCollider->CheckCollidingObject(DestCollider.get()))
				{
					SrcCollider->CallOnCollisionEnd(DestCollider.get());
					DestCollider->CallOnCollisionEnd(SrcCollider.get());
				}
			}

			++DestIt;
		}

		++It;
	}
}

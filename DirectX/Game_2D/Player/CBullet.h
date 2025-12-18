#pragma once
#include <World/CWorld.h>

#include "Object/CGameObject.h"

class CMeshComponent;

class CBullet :
	public CGameObject
{
	friend class CWorld;

public:
	void SetMoveDirection(const FVector& MoveDirection)
	{
		bUseMoveDirection = true;
		this->MoveDirection = MoveDirection;
	}

	template <typename T>
	bool SetCloseTarget()
	{
		auto World = this->World.lock();
		if (!World)
		{
			return false;
		}

		auto Candidates = World->FindObjectsOfType<T>();
		if (Candidates.empty())
		{
			return false;
		}

		bUseMoveDirection = true;

		float SqrDist = FLT_MAX;
		for (const auto& WeakCandidate : Candidates)
		{
			if (auto Candidate = std::dynamic_pointer_cast<CGameObject>(WeakCandidate.lock()))
			{
				FVector3 CurrDir = Candidate->GetWorldPosition() - GetWorldPosition();
				float CurrSqrDist = CurrDir.SqrLength();
				if (CurrSqrDist < SqrDist)
				{
					SqrDist = CurrSqrDist;
					Target = Candidate;
					MoveDirection = CurrDir.GetNormalized();
				}
			}
		}

		return SqrDist != FLT_MAX;
	}

private:
	float Distance = 600;

	FVector MoveDirection;
	bool bUseMoveDirection = false;

	std::weak_ptr<CGameObject> Target;

private:
	std::weak_ptr<CMeshComponent> MeshComponent;

public:
	bool Init() override;
	void Update(const float DeltaTime) override;

protected:
	CBullet() = default;
	CBullet(const CBullet& other) = default;
	CBullet(CBullet&& other) noexcept = default;

public:
	~CBullet() override = default;
};


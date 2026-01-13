#pragma once
#include <World/CWorld.h>

#include "Object/CGameObject.h"

class CColliderBox2D;
class CProjectileMovementComponent;
class CMeshComponent;

class CBullet :
	public CGameObject
{
	friend class CWorld;
	friend CObject;

public:
	void SetMoveDirection(const FVector& MoveDirection);

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

	void CalcCollisionRadius()
	{
		// Original mesh is 1*1*1.
		FVector3 HalfExtent = GetWorldScale() / 2.f;
		CollisionRadius = sqrtf(HalfExtent.x * HalfExtent.x + HalfExtent.y * HalfExtent.y);
	}

	void SetCollision(const std::string& Key);
	void SetCollisionTargetName(const std::string& CollisionTargetName)
	{
		this->CollisionTargetName = CollisionTargetName;
	}

	void SetEnableMove(bool bEnable);

	void SetDistance(float Distance);

	void SetSpeed(float Speed);

private:
	void MoveEndFunction();

	void OnCollisionBegin(const FVector& HitPoint, CCollider* Other);
	void OnCollisionEnd(CCollider* Other);
	void OnCollisionBlock(const FVector& HitPoint, CCollider* Other);

private:
	float Distance = 600;
	float Speed = 300.f;

	FVector MoveDirection;
	bool bUseMoveDirection = false;

	std::weak_ptr<CGameObject> Target;

	std::string CollisionTargetName;
	float CollisionRadius = 0.f;

	bool bEnableMove = true;

private:
	std::weak_ptr<CMeshComponent> MeshComponent;
	std::weak_ptr<CProjectileMovementComponent> MovementComponent;
	std::weak_ptr<CColliderBox2D> Body;

public:
	bool Init() override;
	void Update(const float DeltaTime) override;
	void PostUpdate(const float DeltaTime) override;

protected:
	CBullet* Clone() override;

protected:
	CBullet() = default;
	CBullet(const CBullet& other) = default;
	CBullet(CBullet&& other) noexcept = default;

public:
	~CBullet() override = default;
};


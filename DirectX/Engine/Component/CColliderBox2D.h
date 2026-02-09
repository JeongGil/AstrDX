#pragma once
#include "CCollider.h"

class CColliderBox2D :
    public CCollider
{
	friend class CGameObject;
	friend CObject;

public:
	[[nodiscard]] FBox2DInfo GetInfo() const
	{
		return Info;
	}

	void SetBoxExtent(const FVector2& Extend)
	{
		Info.HalfExtent = Extend * 0.5f;
	}
	void SetBoxExtent(float X, float Y)
	{
		Info.HalfExtent = FVector2(X * 0.5f, Y * 0.5f);
	}
	void SetBoxHalfExtent(const FVector2& HalfExtent)
	{
		Info.HalfExtent = HalfExtent;
	}
	void SetBoxHalfExtent(float X, float Y)
	{
		Info.HalfExtent = FVector2(X, Y);
	}

protected:
	FBox2DInfo Info;

public:
	bool Collide(FVector3& OutHitPoint, std::shared_ptr<CCollider> Other) override;
	bool CollideManifold(FCollisionManifold& HitResult, std::shared_ptr<CCollider> Dest) override;
	bool CollideMouse(const FVector2& MousePos) override;

	void SetDrawDebug(bool bDrawDebug) override;
	bool Init() override;
	void Update(const float DeltaTime) override;
	void PostUpdate(const float DeltaTime) override;
	void UpdateInfo() override;

protected:
	CColliderBox2D* Clone() const override;

protected:
	CColliderBox2D();
	CColliderBox2D(const CColliderBox2D& other);
	CColliderBox2D(CColliderBox2D&& other) noexcept;

public:
	~CColliderBox2D() override = default;
};


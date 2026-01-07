#pragma once
#include "CCollider.h"

class CColliderLine2D :
    public CCollider
{
	friend class CGameObject;
	friend CObject;

public:
	void SetDrawDebug(bool bDrawDebug) override;
	bool Collide(FVector3& OutHitPoint, std::shared_ptr<CCollider> Other) override;
	bool Init() override;
	void Update(const float DeltaTime) override;
	void PostUpdate(const float DeltaTime) override;

protected:
	CColliderLine2D* Clone() const override;

public:
	[[nodiscard]] FLine2DInfo GetInfo() const { return Info; }

	void SetLineDir(const FVector& Dir)
	{
		LineDirection = Dir.GetNormalized();
	}

	void SetLineDir(float x, float y, float z = 0.f)
	{
		LineDirection = FVector(x, y, z).GetNormalized();
	}

	void SetLineDistance(float Distance)
	{
		this->Distance = Distance;
	}

protected:
	FLine2DInfo Info;
	FVector LineDirection;
	float Distance = 100;

protected:
	CColliderLine2D();
	CColliderLine2D(const CColliderLine2D& other) = default;
	CColliderLine2D(CColliderLine2D&& other) noexcept = default;
	CColliderLine2D& operator=(const CColliderLine2D& other) = default;
	CColliderLine2D& operator=(CColliderLine2D&& other) noexcept = default;

public:
	~CColliderLine2D() override = default;
};


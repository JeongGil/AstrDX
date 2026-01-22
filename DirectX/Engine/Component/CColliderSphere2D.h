#pragma once
#include "CCollider.h"

class CColliderSphere2D :
    public CCollider
{

	friend class CGameObject;
	friend CObject;

public:
	bool Init() override;
	void Update(const float DeltaTime) override;
	void PostUpdate(const float DeltaTime) override;
	void UpdateInfo() override;

protected:
	CColliderSphere2D* Clone() const override;

public:
	void SetDrawDebug(bool bDrawDebug) override;
	bool Collide(FVector3& OutHitPoint, std::shared_ptr<CCollider> Other) override;
	bool CollideManifold(FCollisionManifold& HitResult, std::shared_ptr<CCollider> Dest) override;

	[[nodiscard]] FSphere2DInfo GetInfo() const
	{
		return Info;
	}

	void SetRadius(float Radius)
	{
		Info.Radius = Radius;
	}

protected:
	FSphere2DInfo Info;
	
protected:
	CColliderSphere2D();
	CColliderSphere2D(const CColliderSphere2D& other) = default;
	CColliderSphere2D(CColliderSphere2D&& other) noexcept = default;
	CColliderSphere2D& operator=(const CColliderSphere2D& other) = default;
	CColliderSphere2D& operator=(CColliderSphere2D&& other) noexcept = default;

public:
	~CColliderSphere2D() override = default;
};


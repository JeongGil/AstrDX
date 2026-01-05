#pragma once
#include "Component/CCollider.h"

class CColliderBox2D :
    public CCollider
{
	friend class CGameObject;
	friend CObject;

public:
	[[nodiscard]] FBox2DInfo GetInfo() const { return Info; }

	void SetBoxExtend(const FVector2& Extend) { Info.HalfExtent = Extend * 0.5f; }
	void SetBoxExtend(float X, float Y) { Info.HalfExtent = FVector2(X * 0.5f, Y * 0.5f); }
	void SetBoxHalfExtend(const FVector2& HalfExtent) { Info.HalfExtent = HalfExtent; }
	void SetBoxHalfExtend(float X, float Y) { Info.HalfExtent = FVector2(X, Y); }

protected:
	FBox2DInfo Info;

public:
	void SetDrawDebug(bool bDrawDebug) override;
	bool Init() override;
	void Update(const float DeltaTime) override;
	void PostUpdate(const float DeltaTime) override;

protected:
	CColliderBox2D* Clone() const override;

protected:
	CColliderBox2D();

	CColliderBox2D(const CColliderBox2D& other)
		: CCollider(other),
		  Info(other.Info)
	{
	}

	CColliderBox2D(CColliderBox2D&& other) noexcept
		: CCollider(std::move(other)),
		  Info(std::move(other.Info))
	{
	}

	CColliderBox2D& operator=(const CColliderBox2D& other)
	{
		if (this == &other)
			return *this;
		CCollider::operator =(other);
		Info = other.Info;
		return *this;
	}

	CColliderBox2D& operator=(CColliderBox2D&& other) noexcept
	{
		if (this == &other)
			return *this;
		CCollider::operator =(std::move(other));
		Info = std::move(other.Info);
		return *this;
	}

public:
	~CColliderBox2D() override = default;
};


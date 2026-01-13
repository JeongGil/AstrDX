#pragma once
#include "CMovementComponent.h"

class CProjectileMovementComponent :
	public CMovementComponent
{
	friend class CGameObject;

public:
	[[nodiscard]] FVector GetVelocity() const
	{
		return Velocity;
	}

	[[nodiscard]] FVector GetMoveDirection() const
	{
		return MoveDirection;
	}
	void SetMoveDirection(const FVector& Dir)
	{
		MoveDirection = Dir.GetNormalized();
	}

	[[nodiscard]] float GetSpeed() const
	{
		return Speed;
	}
	void SetSpeed(float Speed)
	{
		this->Speed = Speed;
	}

	[[nodiscard]] float GetDeltaMovement() const
	{
		if (MoveDirection.IsZero())
		{
			return 0.f;
		}
		else
		{
			return Speed * CTimer::GetDeltaTime();
		}
	}

	void SetRange(float Range)
	{
		this->Range = Range;
	}

	template <typename T>
	void SetRangeFunction(T* Obj, void (T::* Func)())
	{
		RangeFunction = std::bind(Func, Obj);
	}

protected:
	FVector MoveDirection = FVector::Zero;
	float Speed = 200.f;
	FVector Velocity = FVector::Zero;
	float Range = -1.f;

	std::function<void()> RangeFunction;

public:
	bool Init() override;
	void Update(const float Delta) override;
	void PostUpdate(const float DeltaTime) override;
	void Destroy() override;

protected:
	CProjectileMovementComponent* Clone() const override;

	CProjectileMovementComponent() = default;
	CProjectileMovementComponent(const CProjectileMovementComponent& other) = default;
	CProjectileMovementComponent(CProjectileMovementComponent&& other) noexcept = default;
	CProjectileMovementComponent& operator=(const CProjectileMovementComponent& other) = default;
	CProjectileMovementComponent& operator=(CProjectileMovementComponent&& other) noexcept = default;	

public:
	~CProjectileMovementComponent() override = default;
};


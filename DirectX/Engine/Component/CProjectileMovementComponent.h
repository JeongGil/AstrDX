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

	CProjectileMovementComponent(const CProjectileMovementComponent& other)
		: CMovementComponent(other),
		  MoveDirection(other.MoveDirection),
		  Speed(other.Speed),
		  Velocity(other.Velocity),
		  Range(other.Range),
		  RangeFunction(other.RangeFunction)
	{
	}

	CProjectileMovementComponent(CProjectileMovementComponent&& other) noexcept
		: CMovementComponent(std::move(other)),
		  MoveDirection(std::move(other.MoveDirection)),
		  Speed(other.Speed),
		  Velocity(std::move(other.Velocity)),
		  Range(other.Range),
		  RangeFunction(std::move(other.RangeFunction))
	{
	}

	CProjectileMovementComponent& operator=(const CProjectileMovementComponent& other)
	{
		if (this == &other)
			return *this;
		CMovementComponent::operator =(other);
		MoveDirection = other.MoveDirection;
		Speed = other.Speed;
		Velocity = other.Velocity;
		Range = other.Range;
		RangeFunction = other.RangeFunction;
		return *this;
	}

	CProjectileMovementComponent& operator=(CProjectileMovementComponent&& other) noexcept
	{
		if (this == &other)
			return *this;
		CMovementComponent::operator =(std::move(other));
		MoveDirection = std::move(other.MoveDirection);
		Speed = other.Speed;
		Velocity = std::move(other.Velocity);
		Range = other.Range;
		RangeFunction = std::move(other.RangeFunction);
		return *this;
	}

	CProjectileMovementComponent() = default;

public:
	~CProjectileMovementComponent() override = default;
};


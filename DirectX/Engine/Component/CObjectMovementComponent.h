#pragma once
#include "CMovementComponent.h"

class CObjectMovementComponent :
    public CMovementComponent
{
	friend class CGameObject;

public:
	[[nodiscard]] FVector GetMoveDirection() const { return MoveDirection; }
	void AddMove(const FVector& Dir) { MoveDirection += Dir; }

	[[nodiscard]] float GetSpeed() const { return Speed; }
	void SetSpeed(float Speed) { this->Speed = Speed; }

	[[nodiscard]] float GetDeltaMovement() const { return MoveDirection.IsZero() ? 0.f : Speed * CTimer::GetDeltaTime(); }

protected:
	FVector MoveDirection = FVector::Zero;
	float Speed = 200.f;
	FVector Velocity = FVector::Zero;

public:
	bool Init() override;
	void Update(const float Delta) override;
	void PostUpdate(const float DeltaTime) override;
	void Destroy() override;

protected:
	CObjectMovementComponent* Clone() const override;

protected:
	CObjectMovementComponent() = default;

	CObjectMovementComponent(const CObjectMovementComponent& other)
		: CMovementComponent(other),
		  MoveDirection(other.MoveDirection),
		  Speed(other.Speed),
		  Velocity(other.Velocity)
	{
	}

	CObjectMovementComponent(CObjectMovementComponent&& other) noexcept
		: CMovementComponent(std::move(other)),
		  MoveDirection(std::move(other.MoveDirection)),
		  Speed(other.Speed),
		  Velocity(std::move(other.Velocity))
	{
	}

	CObjectMovementComponent& operator=(const CObjectMovementComponent& other)
	{
		if (this == &other)
			return *this;
		CMovementComponent::operator =(other);
		MoveDirection = other.MoveDirection;
		Speed = other.Speed;
		Velocity = other.Velocity;
		return *this;
	}

	CObjectMovementComponent& operator=(CObjectMovementComponent&& other) noexcept
	{
		if (this == &other)
			return *this;
		CMovementComponent::operator =(std::move(other));
		MoveDirection = std::move(other.MoveDirection);
		Speed = other.Speed;
		Velocity = std::move(other.Velocity);
		return *this;
	}

public:
	~CObjectMovementComponent() override = default;
};


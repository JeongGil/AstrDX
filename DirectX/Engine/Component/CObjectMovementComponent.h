#pragma once
#include "CMovementComponent.h"
#include "../World/CNavAgent.h"

class CObjectMovementComponent :
	public CMovementComponent,
	public CNavAgent
{
	friend class CGameObject;

public:
	[[nodiscard]] FVector GetMoveDirection() const
	{
		return MoveDirection;
	}
	void AddMove(const FVector& Dir)
	{
		MoveDirection += Dir;
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

protected:
	FVector MoveDirection = FVector::Zero;
	float Speed = 200.f;
	FVector Velocity = FVector::Zero;

public:
	bool Init() override;
	void Update(const float Delta) override;
	void PostUpdate(const float DeltaTime) override;
	void PostRender() override;
	void Destroy() override;

	void Move(const FVector& Pos);
	void Move(const FVector2& Pos);
	void MovePath(const FVector& Pos);
	void MovePath(const FVector2& Pos);
	void StartPath() override;

protected:
	CObjectMovementComponent* Clone() const override;

protected:
	CObjectMovementComponent() = default;
	CObjectMovementComponent(const CObjectMovementComponent& other) = default;
	CObjectMovementComponent(CObjectMovementComponent&& other) noexcept = default;
	CObjectMovementComponent& operator=(const CObjectMovementComponent& other) = default;
	CObjectMovementComponent& operator=(CObjectMovementComponent&& other) noexcept = default;

public:
	~CObjectMovementComponent() override = default;
};


#pragma once
#include "CMovementComponent.h"

class CRotationMovementComponent :
	public CMovementComponent
{
public:
	[[nodiscard]] EAxis::Type GetAxis() const { return Axis; }
	void SetAxis(const EAxis::Type axis) { Axis = axis; }

	[[nodiscard]] float GetRotationSpeed() const { return AngularSpeedInDegree; }
	void SetRotationSpeed(const float Degree) { AngularSpeedInDegree = Degree; }

protected:
	EAxis::Type Axis = EAxis::Z;
	float AngularSpeedInDegree = 0.f;

public:
	bool Init() override;
	void Update(const float Delta) override;
	void PostUpdate(const float DeltaTime) override;
	void Destroy() override;

protected:
	CRotationMovementComponent* Clone() const override;
};


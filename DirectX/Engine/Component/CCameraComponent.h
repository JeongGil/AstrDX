#pragma once
#include "CSceneComponent.h"

class CGameObject;

enum class ECameraProjectionType : unsigned char
{
	Perspective,
	Orthogonal,
};

class CCameraComponent :
	public CSceneComponent
{
	friend CGameObject;
	friend CObject;

public:
	[[nodiscard]] const FMatrix& GetViewMatrix() const
	{
		return ViewMatrix;
	}

	[[nodiscard]] const FMatrix& GetProjectionMatrix() const
	{
		return ProjectionMatrix;
	}

	void SetProjection(ECameraProjectionType Type, float ViewAngleDegree, float Width, float Height, float ViewDistance);

protected:
	CCameraComponent* Clone() const override;

protected:
	ECameraProjectionType ProjectionType = ECameraProjectionType::Perspective;

	FMatrix ViewMatrix;
	FMatrix ProjectionMatrix;

	float ViewAngleDegree = 90.f;
	float Width = 1280.f;
	float Height = 720.f;
	float ViewDistance = 1000.f;

public:
	bool Init() override;
	void Update(const float DeltaTime) override;
	void PostUpdate(const float DeltaTime) override;

protected:
	CCameraComponent() = default;
	CCameraComponent(const CCameraComponent& other) = default;
	CCameraComponent(CCameraComponent&& other) noexcept = default;

public:
	~CCameraComponent() override = default;
};


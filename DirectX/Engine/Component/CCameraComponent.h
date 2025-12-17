#pragma once
#include "CSceneComponent.h"


class CCameraComponent :
	public CSceneComponent
{
	friend class CGameObject;

public:
	enum class EProjectionType : unsigned char
	{
		Perspective,
		Orthogonal,
	};

public:
	[[nodiscard]] FMatrix GetViewMatrix() const
	{
		return ViewMatrix;
	}

	[[nodiscard]] FMatrix GetProjectionMatrix() const
	{
		return ProjectionMatrix;
	}

	void SetProjection(EProjectionType Type, float ViewAngleDegree, float Width, float Height, float ViewDistance);

protected:
	EProjectionType ProjectionType = EProjectionType::Perspective;

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

public:
	~CCameraComponent() override = default;
};


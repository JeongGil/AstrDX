#pragma once
#include "CSceneComponent.h"


class CCameraComponent :
	public CSceneComponent
{
	friend class CGameObject;
	friend CObject;

public:
	enum class EProjectionType : unsigned char
	{
		Perspective,
		Orthogonal,
	};

public:
	[[nodiscard]] const FMatrix& GetViewMatrix() const
	{
		return ViewMatrix;
	}

	[[nodiscard]] const FMatrix& GetProjectionMatrix() const
	{
		return ProjectionMatrix;
	}

	void SetProjection(EProjectionType Type, float ViewAngleDegree, float Width, float Height, float ViewDistance);

protected:
	CCameraComponent* Clone() const override;

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

	CCameraComponent(const CCameraComponent& other)
		: CSceneComponent(other),
		  ProjectionType(other.ProjectionType),
		  ViewMatrix(other.ViewMatrix),
		  ProjectionMatrix(other.ProjectionMatrix),
		  ViewAngleDegree(other.ViewAngleDegree),
		  Width(other.Width),
		  Height(other.Height),
		  ViewDistance(other.ViewDistance)
	{
	}

	CCameraComponent(CCameraComponent&& other) noexcept
		: CSceneComponent(std::move(other)),
		  ProjectionType(other.ProjectionType),
		  ViewMatrix(std::move(other.ViewMatrix)),
		  ProjectionMatrix(std::move(other.ProjectionMatrix)),
		  ViewAngleDegree(other.ViewAngleDegree),
		  Width(other.Width),
		  Height(other.Height),
		  ViewDistance(other.ViewDistance)
	{
	}

	CCameraComponent& operator=(const CCameraComponent& other)
	{
		if (this == &other)
			return *this;
		CSceneComponent::operator =(other);
		ProjectionType = other.ProjectionType;
		ViewMatrix = other.ViewMatrix;
		ProjectionMatrix = other.ProjectionMatrix;
		ViewAngleDegree = other.ViewAngleDegree;
		Width = other.Width;
		Height = other.Height;
		ViewDistance = other.ViewDistance;
		return *this;
	}

	CCameraComponent& operator=(CCameraComponent&& other) noexcept
	{
		if (this == &other)
			return *this;
		CSceneComponent::operator =(std::move(other));
		ProjectionType = other.ProjectionType;
		ViewMatrix = std::move(other.ViewMatrix);
		ProjectionMatrix = std::move(other.ProjectionMatrix);
		ViewAngleDegree = other.ViewAngleDegree;
		Width = other.Width;
		Height = other.Height;
		ViewDistance = other.ViewDistance;
		return *this;
	}

public:
	~CCameraComponent() override = default;
};


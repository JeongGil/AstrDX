#pragma once
#include "CComponent.h"

enum class EComponentRender : unsigned char
{
	None,
	Render,
};

class CSceneComponent :
	public CComponent
{
	friend class CGameObject;

public:
	void SetInheritScale(bool bInherit);
	void SetInheritRot(bool bInherit);
	void SetRelativeScale(const FVector3& Scale);
	void SetRelativeScale(const FVector2& Scale);
	void SetRelativeScale(float X, float Y, float Z);
	void SetRelativeScale(float X, float Y);

	void SetRelativeRotation(const FVector3& Rotation);
	void SetRelativeRotation(const FVector2& Rotation);
	void SetRelativeRotation(float X, float Y, float Z);
	void SetRelativeRotation(float X, float Y);
	void SetRelativeRotationX(float X);
	void SetRelativeRotationY(float Y);
	void SetRelativeRotationZ(float Z);

	void SetRelativePosition(const FVector3& Position);
	void SetRelativePosition(const FVector2& Position);
	void SetRelativePosition(float X, float Y, float Z);
	void SetRelativePosition(float X, float Y);

protected:
	std::weak_ptr<CSceneComponent> Parent;
	EComponentRender RenderType = EComponentRender::None;
	std::vector<std::shared_ptr<CSceneComponent>> Children;

	bool bInheritScale = true;
	bool bInheritRotation = true;

	FVector RelativeScale = FVector::One;
	FVector RelativeRotation;
	FVector RelativePosition;

	FVector WorldScale = FVector::One;
	FVector WorldRotation;
	FVector WorldPosition;

	FVector3 WorldAxis[EAxis::End] =
	{
		FVector3::Axis[EAxis::X],
		FVector3::Axis[EAxis::Y],
		FVector3::Axis[EAxis::Z],
	};

	FMatrix ScaleMatrix;
	FMatrix RotationMatrix;
	FMatrix TranslateMatrix;
	FMatrix WorldMatrix;

public:
	bool Init() override;
	void Update(const float Delta) override;
	void Render() override;
	CComponent* Clone() const override;
	void Destroy() override;

protected:
	CSceneComponent() = default;

	CSceneComponent(const CSceneComponent& ref)
		: CComponent(ref)
	{
	}

	CSceneComponent(CSceneComponent&& ref) noexcept
		: CComponent(std::move(ref))
	{
	}

public:
	~CSceneComponent() override = default;
};


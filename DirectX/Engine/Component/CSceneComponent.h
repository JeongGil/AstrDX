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
	void AddChild(const std::weak_ptr<CSceneComponent>& Child);
	void UpdateTransform();

public:
	const FVector& GetAxis(EAxis::Type Axis) const
	{
		return WorldAxis[Axis];
	}

	const FVector& GetRelativeScale() const
	{
		return RelativeScale;
	}

	const FVector& GetRelativeRotation() const
	{
		return RelativeRotation;
	}

	const FVector& GetRelativePosition() const
	{
		return RelativePosition;
	}

	const FVector& GetWorldScale() const
	{
		return WorldScale;
	}

	const FVector& GetWorldRotation() const
	{
		return WorldRotation;
	}

	const FVector& GetWorldPosition() const
	{
		return WorldPosition;
	}

	void SetInheritScale(bool bInherit);
	void SetInheritRot(bool bInherit);

	void SetRelativeScale(const FVector3& Scale);
	void SetRelativeScale(const FVector2& Scale);
	void SetRelativeScale(float X, float Y, float Z);
	void SetRelativeScale(float X, float Y);

	void AddRelativeScale(const FVector3& Scale);
	void AddRelativeScale(const FVector2& Scale);
	void AddRelativeScale(float X, float Y, float Z);
	void AddRelativeScale(float X, float Y);

	void SetRelativeRotation(const FVector3& Rotation);
	void SetRelativeRotation(const FVector2& Rotation);
	void SetRelativeRotation(float X, float Y, float Z);
	void SetRelativeRotation(float X, float Y);
	void SetRelativeRotationX(float X);
	void SetRelativeRotationY(float Y);
	void SetRelativeRotationZ(float Z);

	void AddRelativeRotation(const FVector3& Rotation);
	void AddRelativeRotation(const FVector2& Rotation);
	void AddRelativeRotation(float X, float Y, float Z);
	void AddRelativeRotation(float X, float Y);
	void AddRelativeRotationX(float X);
	void AddRelativeRotationY(float Y);
	void AddRelativeRotationZ(float Z);

	void SetRelativePosition(const FVector3& Position);
	void SetRelativePosition(const FVector2& Position);
	void SetRelativePosition(float X, float Y, float Z);
	void SetRelativePosition(float X, float Y);

	void AddRelativePosition(const FVector3& Position);
	void AddRelativePosition(const FVector2& Position);
	void AddRelativePosition(float X, float Y, float Z);
	void AddRelativePosition(float X, float Y);

	void InheritRelativeScale();
	void InheritRelativeRotation();
	void InheritRelativePosition();

	void SetWorldScale(const FVector3& Scale);
	void SetWorldScale(const FVector2& Scale);
	void SetWorldScale(float X, float Y, float Z);
	void SetWorldScale(float X, float Y);

	void AddWorldScale(const FVector3& Scale);
	void AddWorldScale(const FVector2& Scale);
	void AddWorldScale(float X, float Y, float Z);
	void AddWorldScale(float X, float Y);

	void SetWorldRotation(const FVector3& Rotation);
	void SetWorldRotation(const FVector2& Rotation);
	void SetWorldRotation(float X, float Y, float Z);
	void SetWorldRotation(float X, float Y);
	void SetWorldRotationX(float X);
	void SetWorldRotationY(float Y);
	void SetWorldRotationZ(float Z);

	void AddWorldRotation(const FVector3& Rotation);
	void AddWorldRotation(const FVector2& Rotation);
	void AddWorldRotation(float X, float Y, float Z);
	void AddWorldRotation(float X, float Y);
	void AddWorldRotationX(float X);
	void AddWorldRotationY(float Y);
	void AddWorldRotationZ(float Z);

	void SetWorldPosition(const FVector3& Position);
	void SetWorldPosition(const FVector2& Position);
	void SetWorldPosition(float X, float Y, float Z);
	void SetWorldPosition(float X, float Y);

	void AddWorldPosition(const FVector3& Position);
	void AddWorldPosition(const FVector2& Position);
	void AddWorldPosition(float X, float Y, float Z);
	void AddWorldPosition(float X, float Y);

	void InheritWorldScale();
	void InheritWorldRotation();
	void InheritWorldPosition();

protected:
	std::weak_ptr<CSceneComponent> Parent;
	EComponentRender RenderType = EComponentRender::None;
	std::vector<std::weak_ptr<CSceneComponent>> Children;

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
	void Update(const float DeltaTime) override;
	void Render() override;
	CSceneComponent* Clone() const override;
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


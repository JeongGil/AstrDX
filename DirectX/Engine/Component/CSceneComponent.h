#pragma once
#include "CComponent.h"
#include <array>

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
	const FVector& GetAxis(EAxis::Type Axis) const;
	const FVector& GetRelativeScale() const;
	const FVector& GetRelativeRotation() const;
	const FVector& GetRelativePosition() const;
	const FVector& GetWorldScale() const;
	const FVector& GetWorldRotation() const;
	const FVector& GetWorldPosition() const;

	void SetInheritScale(bool bInherit);
	void SetInheritRotation(bool bInherit);

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

	std::array<FVector, EAxis::End> WorldAxis =
	{
		FVector::Axis[EAxis::X],
		FVector::Axis[EAxis::Y],
		FVector::Axis[EAxis::Z],
	};

	FMatrix ScaleMatrix;
	FMatrix RotationMatrix;
	FMatrix TranslateMatrix;
	FMatrix WorldMatrix;

public:
	bool Init() override;
	void Update(const float DeltaTime) override;
	void PostUpdate(const float DeltaTime) override;
	void Render() override;
	void Destroy() override;

protected:
	CSceneComponent* Clone() const override;

protected:
	CSceneComponent() = default;

	CSceneComponent(const CSceneComponent& other)
		: CComponent(other),
		  //Parent(other.Parent),
		  RenderType(other.RenderType),
		  //Children(other.Children),
		  bInheritScale(other.bInheritScale),
		  bInheritRotation(other.bInheritRotation),
		  RelativeScale(other.RelativeScale),
		  RelativeRotation(other.RelativeRotation),
		  RelativePosition(other.RelativePosition),
		  WorldScale(other.WorldScale),
		  WorldRotation(other.WorldRotation),
		  WorldPosition(other.WorldPosition),
		  WorldAxis(other.WorldAxis),
		  ScaleMatrix(other.ScaleMatrix),
		  RotationMatrix(other.RotationMatrix),
		  TranslateMatrix(other.TranslateMatrix),
		  WorldMatrix(other.WorldMatrix)
	{
	}

	CSceneComponent(CSceneComponent&& other) noexcept
		: CComponent(std::move(other)),
		  //Parent(std::move(other.Parent)),
		  RenderType(other.RenderType),
		  //Children(std::move(other.Children)),
		  bInheritScale(other.bInheritScale),
		  bInheritRotation(other.bInheritRotation),
		  RelativeScale(std::move(other.RelativeScale)),
		  RelativeRotation(std::move(other.RelativeRotation)),
		  RelativePosition(std::move(other.RelativePosition)),
		  WorldScale(std::move(other.WorldScale)),
		  WorldRotation(std::move(other.WorldRotation)),
		  WorldPosition(std::move(other.WorldPosition)),
		  WorldAxis(std::move(other.WorldAxis)),
		  ScaleMatrix(std::move(other.ScaleMatrix)),
		  RotationMatrix(std::move(other.RotationMatrix)),
		  TranslateMatrix(std::move(other.TranslateMatrix)),
		  WorldMatrix(std::move(other.WorldMatrix))
	{
	}

	CSceneComponent& operator=(const CSceneComponent& other)
	{
		if (this == &other)
			return *this;
		CComponent::operator =(other);
		//Parent = other.Parent;
		RenderType = other.RenderType;
		//Children = other.Children;
		bInheritScale = other.bInheritScale;
		bInheritRotation = other.bInheritRotation;
		RelativeScale = other.RelativeScale;
		RelativeRotation = other.RelativeRotation;
		RelativePosition = other.RelativePosition;
		WorldScale = other.WorldScale;
		WorldRotation = other.WorldRotation;
		WorldPosition = other.WorldPosition;
		WorldAxis = other.WorldAxis;
		ScaleMatrix = other.ScaleMatrix;
		RotationMatrix = other.RotationMatrix;
		TranslateMatrix = other.TranslateMatrix;
		WorldMatrix = other.WorldMatrix;
		return *this;
	}

	CSceneComponent& operator=(CSceneComponent&& other) noexcept
	{
		if (this == &other)
			return *this;
		CComponent::operator =(std::move(other));
		//Parent = std::move(other.Parent);
		RenderType = other.RenderType;
		//Children = std::move(other.Children);
		bInheritScale = other.bInheritScale;
		bInheritRotation = other.bInheritRotation;
		RelativeScale = std::move(other.RelativeScale);
		RelativeRotation = std::move(other.RelativeRotation);
		RelativePosition = std::move(other.RelativePosition);
		WorldScale = std::move(other.WorldScale);
		WorldRotation = std::move(other.WorldRotation);
		WorldPosition = std::move(other.WorldPosition);
		WorldAxis = std::move(other.WorldAxis);
		ScaleMatrix = std::move(other.ScaleMatrix);
		RotationMatrix = std::move(other.RotationMatrix);
		TranslateMatrix = std::move(other.TranslateMatrix);
		WorldMatrix = std::move(other.WorldMatrix);
		return *this;
	}

public:
	~CSceneComponent() override = default;
};


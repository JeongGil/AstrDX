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
	/**
	 * @brief Adds a child scene component to the current component.
	 *
	 * This method establishes a parent-child relationship between the current
	 * scene component and the specified child component. The child's relative
	 * transformations (scale, rotation, and position) are updated to match
	 * those of the parent component.
	 *
	 * @param Child A weak pointer to the child scene component to be added.
	 *              If the child component is valid, it will be added to the
	 *              list of children for this component.
	 */
	void AddChild(const std::weak_ptr<CSceneComponent>& Child);

	/**
	 * @brief Updates the transformation matrices of the scene component.
	 *
	 * This method recalculates the component's world transformation matrices, including
	 * scaling, rotation, and translation, based on the current world scale, rotation,
	 * and position. The resulting transformation is stored in the world matrix.
	 *
	 * This function is typically called during the update cycle to ensure that the
	 * component's transformations are up-to-date before rendering or further processing.
	 */
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

	void SetRelativeScale(const FVector& Scale);
	void SetRelativeScale(const FVector2& Scale);
	void SetRelativeScale(float X, float Y, float Z);
	void SetRelativeScale(float X, float Y);

	void AddRelativeScale(const FVector& Scale);
	void AddRelativeScale(const FVector2& Scale);
	void AddRelativeScale(float X, float Y, float Z);
	void AddRelativeScale(float X, float Y);

	void SetRelativeRotation(const FVector& Rotation);
	void SetRelativeRotation(const FVector2& Rotation);
	void SetRelativeRotation(float X, float Y, float Z);
	void SetRelativeRotation(float X, float Y);
	void SetRelativeRotationX(float X);
	void SetRelativeRotationY(float Y);
	void SetRelativeRotationZ(float Z);

	void AddRelativeRotation(const FVector& Rotation);
	void AddRelativeRotation(const FVector2& Rotation);
	void AddRelativeRotation(float X, float Y, float Z);
	void AddRelativeRotation(float X, float Y);
	void AddRelativeRotationX(float X);
	void AddRelativeRotationY(float Y);
	void AddRelativeRotationZ(float Z);

	void SetRelativePosition(const FVector& Position);
	void SetRelativePosition(const FVector2& Position);
	void SetRelativePosition(float X, float Y, float Z);
	void SetRelativePosition(float X, float Y);

	void AddRelativePosition(const FVector& Position);
	void AddRelativePosition(const FVector2& Position);
	void AddRelativePosition(float X, float Y, float Z);
	void AddRelativePosition(float X, float Y);

	void InheritRelativeScale();
	void InheritRelativeRotation();
	void InheritRelativePosition();

	void SetWorldScale(const FVector& Scale);
	void SetWorldScale(const FVector2& Scale);
	void SetWorldScale(float X, float Y, float Z);
	void SetWorldScale(float X, float Y);

	void AddWorldScale(const FVector& Scale);
	void AddWorldScale(const FVector2& Scale);
	void AddWorldScale(float X, float Y, float Z);
	void AddWorldScale(float X, float Y);

	void SetWorldRotation(const FVector& Rotation);
	void SetWorldRotation(const FVector2& Rotation);
	void SetWorldRotation(float X, float Y, float Z);
	void SetWorldRotation(float X, float Y);
	void SetWorldRotationX(float X);
	void SetWorldRotationY(float Y);
	void SetWorldRotationZ(float Z);

	void AddWorldRotation(const FVector& Rotation);
	void AddWorldRotation(const FVector2& Rotation);
	void AddWorldRotation(float X, float Y, float Z);
	void AddWorldRotation(float X, float Y);
	void AddWorldRotationX(float X);
	void AddWorldRotationY(float Y);
	void AddWorldRotationZ(float Z);

	void SetWorldPosition(const FVector& Position);
	void SetWorldPosition(const FVector2& Position);
	void SetWorldPosition(float X, float Y, float Z);
	void SetWorldPosition(float X, float Y);

	void AddWorldPosition(const FVector& Position);
	void AddWorldPosition(const FVector2& Position);
	void AddWorldPosition(float X, float Y, float Z);
	void AddWorldPosition(float X, float Y);

	void UpdateChildWorldPosition(const FVector& DeltaPosition);

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

	/**
	 * @brief Represents the world transformation matrix of the scene component.
	 *
	 * This matrix encapsulates the combined transformations of scaling, rotation,
	 * and translation in world space. It is computed based on the component's
	 * relative transformations and any inherited transformations from parent components.
	 *
	 * The world matrix is used for rendering and spatial calculations, ensuring
	 * the component is correctly positioned and oriented in the 3D world.
	 */
	FMatrix WorldMatrix;

public:
	bool Init() override;
	void Update(const float DeltaTime) override;

	/**
	 * @brief Performs post-update operations for the scene component and its children.
	 *
	 * This method is called after the main update to apply transformations and propagate
	 * updates to all child components in the hierarchy. It ensures that the component's
	 * transform is updated and recursively calls the same method on all child components.
	 *
	 * @param DeltaTime The time elapsed since the last update, used for time-dependent operations.
	 */
	void PostUpdate(const float DeltaTime) override;

	/**
	 * @brief Renders the scene component and its child components.
	 *
	 * This method is responsible for rendering the current scene component and
	 * recursively rendering all its child components. It ensures that the entire
	 * hierarchy of components is rendered in the correct order.
	 *
	 * This function is typically called during the rendering phase of the game loop
	 * to draw the component and its children to the screen.
	 */
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


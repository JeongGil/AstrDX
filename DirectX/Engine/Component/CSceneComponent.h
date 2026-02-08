#pragma once
#include "CComponent.h"
#include <array>

enum class EComponentRender : unsigned char
{
	None,
	Render,
};

namespace ERenderOrder
{
	enum Type
	{
		Background = -100,

		Map = 0,
		Default = 1,

		CharacterLeg = 99,
		CharacterBody = 100,
		CharacterDeco = 101,
		CharacterItem = 102,
		CharacterWeapon = 103,

		DebugDraw = 1000,
	};
}

class CSceneComponent :
	public CComponent
{
	friend class CGameObject;
	friend CObject;

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

	[[nodiscard]] EComponentRender GetRenderType() const
	{
		return RenderType;
	}

	[[nodiscard]] int GetRenderLayer() const
	{
		return RenderLayer;
	}

	void SetRenderLayer(const int RenderLayer)
	{
		this->RenderLayer = RenderLayer;
	}

	void SetRenderLayer(const std::string& Name);

	bool TrySetRenderLayer(const int NewRenderLayer);

	const FVector& GetAxis(EAxis::Type Axis) const;
	const FVector& GetRelativeScale() const;
	const FVector& GetRelativeRotation() const;
	const FVector& GetRelativePosition() const;
	const FVector& GetWorldScale() const;
	const FVector& GetWorldRotation() const;
	const FVector& GetWorldPosition() const;

	[[nodiscard]] const FVector& GetVelocity() const
	{
		return Velocity;
	}
	[[nodiscard]] float GetSpeed() const
	{
		return Velocity.Length();
	}

	[[nodiscard]] const FVector& GetPivot() const
	{
		return Pivot;
	}
	void SetPivot(const FVector& Pivot)
	{
		this->Pivot = Pivot;
	}
	void SetPivot(const FVector2& Pivot)
	{
		this->Pivot = FVector(Pivot.x, Pivot.y, 0.f);
	}
	void SetPivot(float x, float y, float z)
	{
		Pivot = FVector(x, y, z);
	}
	void SetPivot(float x, float y)
	{
		Pivot = FVector(x, y, 0.f);
	}

	bool GetSimulatePhysics() const
	{
		return bSimulatePhysics;
	}

	void SetSimulatePhysics(bool bSimulate)
	{
		bSimulatePhysics = bSimulate;
	}

	bool GetUseGravity() const
	{
		return bUseGravity;
	}

	void SetUseGravity(bool bUse)
	{
		bUseGravity = bUse;
	}

	void AddForce(const FVector& Force)
	{
		Accel += Force * (1.f / max(Mass, 0.0001f));
	}

	void ClearPhysics()
	{
		Accel = FVector::Zero;
		PhysicsVelocity = FVector::Zero;
	}

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
	int RenderLayer = 0;
	std::vector<std::weak_ptr<CSceneComponent>> Children;

	bool bInheritScale = true;
	bool bInheritRotation = true;

	FVector Pivot;

	FVector RelativeScale = FVector::One;
	FVector RelativeRotation;
	FVector RelativePosition;

	FVector WorldScale = FVector::One;
	FVector WorldRotation;
	FVector WorldPosition;

	FVector Velocity;

	bool bSimulatePhysics = false;
	bool bUseGravity = false;
	float Mass = 1.f;
	FVector Accel;
	FVector PhysicsVelocity;

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
	void Begin() override;
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
	virtual void PostRender();
	void Destroy() override;

protected:
	CSceneComponent* Clone() const override;

protected:
	CSceneComponent();
	CSceneComponent(const CSceneComponent& other);
	CSceneComponent(CSceneComponent&& other) noexcept;

public:
	~CSceneComponent() override = default;
};


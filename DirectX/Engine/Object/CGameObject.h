#pragma once
#include "../CObject.h"
#include "../Component/CSceneComponent.h"

class CSceneComponent;
class CWorld;

class CGameObject :
	public CObject
{
	friend class CWorld;

public:
	std::weak_ptr<CWorld> GetWorld() const
	{
		return World;
	}

	void SetWorld(const std::weak_ptr<CWorld>& World);

	std::weak_ptr<CSceneComponent> GetRootComponent() const
	{
		return Root;
	}

	void SetName(const std::string& Name)
	{
		this->Name = Name;
	}

	bool GetAlive() const
	{
		return bAlive;
	}

public:
	const FVector& GetAxis(EAxis::Type Axis) const;
	const FVector& GetRelativeScale() const;
	const FVector& GetRelativeRotation() const;
	const FVector& GetRelativePosition() const;
	const FVector& GetWorldScale() const;
	const FVector& GetWorldRotation() const;
	const FVector& GetWorldPosition() const;

	void SetInheritScale(bool bInherit) const;
	void SetInheritRotation(bool bInherit) const;

	void SetRelativeScale(const FVector3& Scale) const;
	void SetRelativeScale(const FVector2& Scale) const;
	void SetRelativeScale(float X, float Y, float Z) const;
	void SetRelativeScale(float X, float Y) const;

	void AddRelativeScale(const FVector3& Scale) const;
	void AddRelativeScale(const FVector2& Scale) const;
	void AddRelativeScale(float X, float Y, float Z) const;
	void AddRelativeScale(float X, float Y) const;

	void SetRelativeRotation(const FVector3& Rotation) const;
	void SetRelativeRotation(const FVector2& Rotation) const;
	void SetRelativeRotation(float X, float Y, float Z) const;
	void SetRelativeRotation(float X, float Y) const;
	void SetRelativeRotationX(float X) const;
	void SetRelativeRotationY(float Y) const;
	void SetRelativeRotationZ(float Z) const;

	void AddRelativeRotation(const FVector3& Rotation) const;
	void AddRelativeRotation(const FVector2& Rotation) const;
	void AddRelativeRotation(float X, float Y, float Z) const;
	void AddRelativeRotation(float X, float Y) const;
	void AddRelativeRotationX(float X) const;
	void AddRelativeRotationY(float Y) const;
	void AddRelativeRotationZ(float Z) const;

	void SetRelativePosition(const FVector3& Position) const;
	void SetRelativePosition(const FVector2& Position) const;
	void SetRelativePosition(float X, float Y, float Z) const;
	void SetRelativePosition(float X, float Y) const;

	void AddRelativePosition(const FVector3& Position) const;
	void AddRelativePosition(const FVector2& Position) const;
	void AddRelativePosition(float X, float Y, float Z) const;
	void AddRelativePosition(float X, float Y) const;

	void SetWorldScale(const FVector3& Scale) const;
	void SetWorldScale(const FVector2& Scale) const;
	void SetWorldScale(float X, float Y, float Z) const;
	void SetWorldScale(float X, float Y) const;

	void AddWorldScale(const FVector3& Scale) const;
	void AddWorldScale(const FVector2& Scale) const;
	void AddWorldScale(float X, float Y, float Z) const;
	void AddWorldScale(float X, float Y) const;

	void SetWorldRotation(const FVector3& Rotation) const;
	void SetWorldRotation(const FVector2& Rotation) const;
	void SetWorldRotation(float X, float Y, float Z) const;
	void SetWorldRotation(float X, float Y) const;
	void SetWorldRotationX(float X) const;
	void SetWorldRotationY(float Y) const;
	void SetWorldRotationZ(float Z) const;

	void AddWorldRotation(const FVector3& Rotation) const;
	void AddWorldRotation(const FVector2& Rotation) const;
	void AddWorldRotation(float X, float Y, float Z) const;
	void AddWorldRotation(float X, float Y) const;
	void AddWorldRotationX(float X) const;
	void AddWorldRotationY(float Y) const;
	void AddWorldRotationZ(float Z) const;

	void SetWorldPosition(const FVector3& Position) const;
	void SetWorldPosition(const FVector2& Position) const;
	void SetWorldPosition(float X, float Y, float Z) const;
	void SetWorldPosition(float X, float Y) const;

	void AddWorldPosition(const FVector3& Position) const;
	void AddWorldPosition(const FVector2& Position) const;
	void AddWorldPosition(float X, float Y, float Z) const;
	void AddWorldPosition(float X, float Y) const;

protected:
	std::weak_ptr<CWorld> World;
	std::vector<std::shared_ptr<CSceneComponent>> SceneComponents;
	std::weak_ptr<CSceneComponent> Root;
	std::string Name;
	bool bAlive = true;

public:
	virtual bool Init();
	virtual void Update(const float DeltaTime);
	virtual void PostUpdate(const float DeltaTime);
	virtual void Render();
	virtual void Destroy();

protected:
	virtual CGameObject* Clone();

public:
	/**
	 * @brief Creates a new component of the specified type and attaches it to the game object.
	 *
	 * This template method allows the creation of a component of type `T` and associates it with
	 * the current game object. The component is initialized with the provided name and optionally
	 * attached to a parent component specified by its name.
	 *
	 * @tparam T The type of the component to be created. Must be derived from `CSceneComponent`.
	 * @param Name The name of the new component.
	 * @param ParentName The name of the parent component to which the new component will be attached.
	 *                   Defaults to "Root" if no parent is specified.
	 * @return A weak pointer to the newly created component of type `T`. Returns an empty weak pointer
	 *         if the component initialization fails.
	 *
	 * @note The created component is automatically added to the game object's list of components.
	 *       If the parent component is not found, the new component is attached to the root component.
	 *       Ensure that the type `T` has a default constructor and implements the `Init` method.
	 */
	template <typename T>
	std::weak_ptr<T> CreateComponent(const std::string& Name, const std::string& ParentName = "Root")
	{
		std::shared_ptr<CSceneComponent> Component(new T);

		Component->SetWorld(World);
		Component->SetOwner(std::static_pointer_cast<CGameObject>(shared_from_this()));
		Component->SetName(Name);

		if (!Component->Init())
		{
			return std::weak_ptr<T>();
		}

		if (SceneComponents.empty())
		{
			Root = Component;
		}
		else
		{
			if (ParentName == "Root")
			{
				if (auto Root = this->Root.lock())
				{
					Root->AddChild(Component);
				}
				else
				{
					std::shared_ptr<CSceneComponent> Parent;
					for (auto SceneComponent : SceneComponents)
					{
						Parent = std::static_pointer_cast<CSceneComponent>(SceneComponent->shared_from_this());
						break;
					}

					if (!Parent)
					{
						if (auto Root = this->Root.lock())
						{
							Root->AddChild(Component);
						}
					}
					else
					{
						Parent->AddChild(Component);
					}
				}
			}
		}

		SceneComponents.push_back(Component);

		return std::dynamic_pointer_cast<T>(Component);
	}

protected:
	CGameObject() = default;

	/**
	 * @brief Clones the hierarchy of scene components from another game object and sets them for the current object.
	 *
	 * This method duplicates the scene components of the specified game object, maintaining their hierarchy
	 * relationships, and assigns them to the current game object. The root component and parent-child relationships
	 * are preserved during the cloning process.
	 *
	 * @param other The game object whose scene components and hierarchy are to be cloned.
	 *
	 * @note This method ensures that the cloned components are properly initialized and linked to the current game object.
	 *       It is typically used in copy constructors or assignment operators to replicate the state of another game object.
	 */
	void CloneAndSetHierarchyComponents(const CGameObject& other)
	{
		// Copy and set root.
		for (const auto& OtherCmp : other.SceneComponents)
		{
			std::shared_ptr<CSceneComponent> Cloned(OtherCmp->Clone());
			SceneComponents.push_back(Cloned);

			auto OtherRoot = other.Root.lock();

			if (OtherCmp == OtherRoot)
			{
				Root = Cloned;
			}
		}

		// Set hierarchy.
		size_t Size = other.SceneComponents.size();
		for (size_t i = 0; i < Size; i++)
		{
			const auto OtherCmp = other.SceneComponents[i];
			if (OtherCmp->Parent.expired())
			{
				continue;
			}

			auto OtherParent = OtherCmp->Parent.lock();
			for (size_t j = 0; j < Size; j++)
			{
				// Find parent idx from other components.
				if (OtherParent == other.SceneComponents[j])
				{
					// Set parent-child using idx.
					SceneComponents[j]->AddChild(SceneComponents[i]);

					break;
				}
			}
		}
	}

	CGameObject(const CGameObject& other)
		: CObject(other),
		World(other.World),
		//SceneComponents(other.SceneComponents),
		//Root(other.Root),
		Name(other.Name),
		bAlive(other.bAlive)
	{
		CloneAndSetHierarchyComponents(other);
	}

	CGameObject(CGameObject&& other) noexcept
		: CObject(std::move(other)),
		World(std::move(other.World)),
		SceneComponents(std::move(other.SceneComponents)),
		Root(std::move(other.Root)),
		Name(std::move(other.Name)),
		bAlive(other.bAlive)
	{
		other.SceneComponents.clear();
		other.Root.reset();
		other.bAlive = false;
	}

	CGameObject& operator=(const CGameObject& other)
	{
		if (this == &other)
			return *this;
		CObject::operator =(other);
		World = other.World;
		//SceneComponents = other.SceneComponents;
		//Root = other.Root;
		CloneAndSetHierarchyComponents(other);
		Name = other.Name;
		bAlive = other.bAlive;
		return *this;
	}

	CGameObject& operator=(CGameObject&& other) noexcept
	{
		if (this == &other)
			return *this;
		CObject::operator =(std::move(other));
		World = std::move(other.World);
		SceneComponents = std::move(other.SceneComponents);
		Root = std::move(other.Root);
		Name = std::move(other.Name);
		bAlive = other.bAlive;
		other.bAlive = false;
		return *this;
	}

public:
	~CGameObject() override = default;
};


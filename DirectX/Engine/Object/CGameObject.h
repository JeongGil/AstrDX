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
				if (const auto Root = this->Root.lock())
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
			const auto& OtherCmp = other.SceneComponents[i];
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
		return *this;
	}

public:
	~CGameObject() override = default;
};


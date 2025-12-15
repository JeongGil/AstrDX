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
	void SetName(const std::string& Name)
	{
		this->Name = Name;
	}

	std::weak_ptr<CWorld> GetWorld() const
	{
		return World;
	}

	void SetWorld(const std::weak_ptr<CWorld>& World)
	{
		this->World = World;
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
	virtual void Update(float DeltaTime);
	virtual void Render();
	virtual CGameObject* Clone();
	virtual void Destroy();

public:
	template <typename T>
	std::weak_ptr<CSceneComponent> CreateComponent(const std::string& Name, const std::string& ParentName = "Root")
	{
		std::shared_ptr<CSceneComponent> Component(new T);

		Component->SetWorld(World);
		Component->SetOwner(std::static_pointer_cast<CGameObject>(shared_from_this()));
		Component->SetName(Name);

		if (!Component->Init())
		{
			return std::weak_ptr<CSceneComponent>();
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

		return Component;
	}

protected:
	CGameObject();
	CGameObject(const CObject& ref)
		: CObject(ref)
	{
	}

	CGameObject(CObject&& ref)
		: CObject(std::move(ref))
	{
	}

public:
	virtual ~CGameObject();
};


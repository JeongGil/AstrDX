#pragma once
#include "../CObject.h"

class CWorld;
class CGameObject;

class CComponent :
	public CObject
{
	friend class CGameObject;

public:
	bool GetAlive() const
	{
		return bAlive;
	}

	std::weak_ptr<CWorld> GetWorld() const
	{
		return World;
	}

	std::weak_ptr<CGameObject> GetOwner() const
	{
		return Owner;
	}

	void SetWorld(std::weak_ptr<CWorld> World)
	{
		this->World = World;
	}

	void SetOwner(std::weak_ptr<CGameObject> Owner)
	{
		this->Owner = Owner;
	}

	void SetName(const std::string& Name)
	{
		this->Name = Name;
	}

protected:
	std::weak_ptr<CWorld> World;
	std::weak_ptr<CGameObject> Owner;
	std::string Name;
	bool bAlive = true;

public:
	virtual bool Init();
	virtual void Update(const float Delta);
	virtual void Render();
	virtual CComponent* Clone() const = 0;
	virtual void Destroy();

protected:
	CComponent() = default;

	CComponent(const CComponent& Other)
		: CObject(Other),
		World(Other.World),
		Owner(Other.Owner),
		Name(Other.Name)
	{
	}

	CComponent(CComponent&& Other) noexcept
		: CObject(std::move(Other)),
		World(std::move(Other.World)),
		Owner(std::move(Other.Owner))

	{
		Other.World.reset();
		Other.Owner.reset();
	}

public:
	~CComponent() override = 0;
};


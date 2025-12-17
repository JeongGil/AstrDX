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

	void SetWorld(std::weak_ptr<CWorld> World)
	{
		this->World = World;
	}

	std::weak_ptr<CGameObject> GetOwner() const
	{
		return Owner;
	}

	void SetOwner(std::weak_ptr<CGameObject> Owner)
	{
		this->Owner = Owner;
	}

	const std::string& GetName() const
	{
		return Name;
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
	virtual void PostUpdate(const float Delta);
	virtual void Render();
	virtual void Destroy();

protected:
	virtual CComponent* Clone() const = 0;

protected:
	CComponent() = default;

	CComponent(const CComponent& other)
		: CObject(other),
		  //World(other.World),
		  //Owner(other.Owner),
		  Name(other.Name),
		  bAlive(other.bAlive)
	{
	}

	CComponent(CComponent&& other) noexcept
		: CObject(std::move(other)),
		  //World(std::move(other.World)),
		  //Owner(std::move(other.Owner)),
		  Name(std::move(other.Name)),
		  bAlive(other.bAlive)
	{
	}

	CComponent& operator=(const CComponent& other)
	{
		if (this == &other)
			return *this;
		CObject::operator =(other);
		//World = other.World;
		//Owner = other.Owner;
		Name = other.Name;
		bAlive = other.bAlive;
		return *this;
	}

	CComponent& operator=(CComponent&& other) noexcept
	{
		if (this == &other)
			return *this;
		CObject::operator =(std::move(other));
		//World = std::move(other.World);
		//Owner = std::move(other.Owner);
		Name = std::move(other.Name);
		bAlive = other.bAlive;
		return *this;
	}

public:
	~CComponent() override;
};


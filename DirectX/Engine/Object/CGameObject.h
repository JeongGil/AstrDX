#pragma once
#include "../CObject.h"

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
	std::string Name;
	std::weak_ptr<CWorld> World;
	bool bAlive = true;

public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void Render();
	virtual CGameObject* Clone();
	virtual void Destroy();

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


#pragma once
#include "../CObject.h"

class CWorld;
class CGameObject;

class CComponent :
	public CObject
{
	friend class CGameObject;

public:
	enum class EType : unsigned char
	{
		None,
		Scene,
		Object,
	};

public:
	bool GetAlive() const
	{
		return bAlive;
	}

	std::weak_ptr<CWorld> GetWorld() const {
		return World; }

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

	const std::string& GetKey() const
	{
		return Key;
	}

	void SetKey(const std::string& Key)
	{
		this->Key = Key;
	}

	[[nodiscard]] EType GetType() const
	{
		return Type;
	}

	bool GetEnable() const
	{
		return bEnable;
	}
	void SetEnable(bool bEnable)
	{
		this->bEnable = bEnable;
	}

protected:
	std::weak_ptr<CWorld> World;
	std::weak_ptr<CGameObject> Owner;
	std::string Key;
	bool bAlive = true;
	bool bEnable = true;
	EType Type = EType::None;

public:
	virtual void Begin();
	virtual bool Init();
	virtual void Update(const float Delta);
	virtual void PostUpdate(const float Delta);
	virtual void Render();
	virtual void Destroy();

protected:
	virtual CComponent* Clone() const = 0;

protected:
	CComponent() = default;
	CComponent(const CComponent& other);
	CComponent(CComponent&& other) noexcept;

public:
	~CComponent() override;
};


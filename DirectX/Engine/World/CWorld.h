#pragma once

#include "../EngineInfo.h"
#include "../Object/CGameObject.h"

class CWorld : public std::enable_shared_from_this<CWorld>
{
public:
	template <typename T>
	std::weak_ptr<T> CreateGameObject(const std::string& Name)
	{
		std::shared_ptr<T> NewObject(new T);

		NewObject->SetWorld(weak_from_this());
		NewObject->SetName(Name);

		if (!NewObject->Init())
		{
			return std::weak_ptr<T>();
		}

		Objects.push_back(NewObject);

		return NewObject;
	}

	template <typename T>
	std::weak_ptr<T> CreateCloneGameObject(const std::string& Name, const std::weak_ptr<T>& WeakOrigin)
	{
		if (WeakOrigin.expired())
		{
			return std::weak_ptr<T>();
		}

		const auto& Origin = WeakOrigin.lock();
		if (!Origin)
		{
			return std::weak_ptr<T>();
		}

		std::shared_ptr<T> NewObject;
		NewObject.reset(Origin->Clone());

		NewObject->SetName(Name);

		Objects.push_back(NewObject);

		return NewObject;
	}

protected:
	std::list<std::shared_ptr<CGameObject>> Objects;

public:
	virtual bool Init();
	virtual void Update(const float DeltaTime);
	virtual void PostUpdate(const float DeltaTime);
	virtual void Render();

public:
	CWorld();
	virtual ~CWorld();
};


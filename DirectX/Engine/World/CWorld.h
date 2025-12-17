#pragma once

#include "CCameraManager.h"
#include "../EngineInfo.h"
#include "../Object/CGameObject.h"

class CWorld : public std::enable_shared_from_this<CWorld>
{
public:
	/**
	 * @brief Creates a new game object of the specified type and assigns it to the world.
	 *
	 * This method creates a new instance of the specified game object type, sets its name,
	 * associates it with the current world, and initializes it. If the initialization fails,
	 * the method returns an empty weak pointer. Otherwise, the created game object is added
	 * to the world's object list and a weak pointer to the object is returned.
	 *
	 * @tparam T The type of the game object to create. Must be a class derived from `CGameObject`.
	 * @param Name The name to assign to the created game object.
	 * @return std::weak_ptr<T> A weak pointer to the created game object, or an empty weak pointer if initialization fails.
	 */
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

	/**
	 * @brief Creates a clone of the specified game object and assigns it to the world.
	 *
	 * This method creates a new instance of the specified game object type by cloning the provided
	 * origin object. The cloned object is assigned a new name, added to the world's object list,
	 * and returned as a weak pointer. If the origin object is expired or invalid, the method
	 * returns an empty weak pointer.
	 *
	 * @tparam T The type of the game object to clone. Must be a class derived from `CGameObject`.
	 * @param Name The name to assign to the cloned game object.
	 * @param WeakOrigin A weak pointer to the origin game object to be cloned.
	 * @return std::weak_ptr<T> A weak pointer to the cloned game object, or an empty weak pointer if cloning fails.
	 */
	template <typename T>
	std::weak_ptr<T> CreateCloneGameObject(const std::string& Name, const std::weak_ptr<T>& WeakOrigin)
	{
		if (WeakOrigin.expired())
		{
			return std::weak_ptr<T>();
		}

		auto Origin = WeakOrigin.lock();
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

	[[nodiscard]] std::weak_ptr<CCameraManager> GetCameraManager() const
	{
		return CameraManager;
	}

protected:
	std::list<std::shared_ptr<CGameObject>> Objects;
	std::shared_ptr<CCameraManager> CameraManager;

public:
	virtual bool Init();
	virtual void Update(const float DeltaTime);
	virtual void PostUpdate(const float DeltaTime);
	virtual void Render();

public:
	CWorld();
	virtual ~CWorld();
};


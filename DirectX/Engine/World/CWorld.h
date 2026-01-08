#pragma once

#include <ranges>

#include "CCameraManager.h"
#include "CInput.h"
#include "CWorldAssetManager.h"
#include "CWorldCollision.h"
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
	 * @param Key The name to assign to the created game object.
	 * @return std::weak_ptr<T> A weak pointer to the created game object, or an empty weak pointer if initialization fails.
	 */
	template <typename T>
	std::weak_ptr<T> CreateGameObject(const std::string& Key)
	{
		std::shared_ptr<T> NewObject(new T);

		NewObject->SetWorld(weak_from_this());
		NewObject->SetName(Key);

		if (!NewObject->Init())
		{
			return std::weak_ptr<T>();
		}

		Objects.emplace(Key, NewObject);
		StartObjects.push_back(NewObject);

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
	 * @param key The name to assign to the cloned game object.
	 * @param WeakOrigin A weak pointer to the origin game object to be cloned.
	 * @return std::weak_ptr<T> A weak pointer to the cloned game object, or an empty weak pointer if cloning fails.
	 */
	template <typename T>
	std::weak_ptr<T> CreateCloneGameObject(const std::string& key, const std::weak_ptr<T>& WeakOrigin)
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

		NewObject->SetWorld(weak_from_this());
		NewObject->SetName(key);

		if (!NewObject->Init())
		{
			return std::weak_ptr<T>();
		}

		Objects.emplace(key, NewObject);
		StartObjects.push_back(NewObject);

		return NewObject;
	}

	std::weak_ptr<CCameraManager> GetCameraManager() const
	{
		return CameraManager;
	}

	template <typename T>
	std::weak_ptr<T> FindObject(const std::string& Key)
	{
		const auto It = Objects.find(Key);
		if (It == Objects.end())
		{
			return std::weak_ptr<T>();
		}
		else
		{
			return std::dynamic_pointer_cast<T>(It->second);
		}
	}

	template <typename T>
	bool FindObjects(const std::string& Key, std::list<std::weak_ptr<T>>& OutObjects)
	{
		auto Range = Objects.equal_range(Key);

		if (Range.first == Range.second)
		{
			return false;
		}

		for (const auto& Pair : std::ranges::subrange(Range.first, Range.second))
		{
			OutObjects.push_back(std::dynamic_pointer_cast<T>(Pair.second));
		}

		return true;
	}

	template <typename T>
	std::vector<std::weak_ptr<T>> FindObjectsOfType()
	{
		std::vector<std::weak_ptr<T>> Results;
		for (const auto& Object : Objects | std::views::values)
		{
			if (auto Casted = std::dynamic_pointer_cast<T>(Object))
			{
				Results.push_back(Casted);
			}
		}

		return Results;
	}

	template <typename T>
	std::weak_ptr<T> FindObjectOfType()
	{
		for (const auto& Object : Objects | std::views::values)
		{
			if (auto Casted = std::dynamic_pointer_cast<T>(Object))
			{
				return Casted;
			}
		}

		return std::weak_ptr<T>();
	}

	std::weak_ptr<CWorldAssetManager> GetWorldAssetManager() const
	{
		return WorldAssetManager;
	}

	std::weak_ptr<CInput> GetInput() const
	{
		return Input;
	}

	std::weak_ptr<CWorldCollision> GetCollision() const
	{
		return Collision;
	}

protected:
	std::unordered_multimap<std::string, std::shared_ptr<CGameObject>> Objects;
	std::vector<std::weak_ptr<CGameObject>> StartObjects;
	std::shared_ptr<CCameraManager> CameraManager;
	std::shared_ptr<CWorldAssetManager> WorldAssetManager;
	std::shared_ptr<CInput> Input;
	std::shared_ptr<CWorldCollision> Collision;

public:
	virtual bool Init();
	virtual void Update(const float DeltaTime);
	virtual void PostUpdate(const float DeltaTime);
	virtual void Render();

private:
	void Begin();

public:
	CWorld();
	virtual ~CWorld();
};


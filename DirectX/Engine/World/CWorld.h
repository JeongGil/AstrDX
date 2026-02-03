#pragma once

#include <ranges>

#include "CCameraManager.h"
#include "CInput.h"
#include "CWorldAssetManager.h"
#include "CWorldCollision.h"
#include "../EngineInfo.h"
#include "../Object/CGameObject.h"
#include "CWorldUIManager.h"

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
		NewObject->SetKey(Key);

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
		NewObject->SetKey(key);

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
		if (auto It = Objects.find(Key); It != Objects.end())
		{
			return std::dynamic_pointer_cast<T>(It->second);
		}

		return {};
	}

	template <typename T>
	bool FindObjects(const std::string& Key, std::list<std::weak_ptr<T>>& OutObjects)
	{
		auto [First, Last] = Objects.equal_range(Key);

		if (First == Last)
		{
			return false;
		}

		auto CastedView = std::ranges::subrange(First, Last)
			| std::views::values
			| std::views::transform([](const auto& Obj)
				{
					return std::dynamic_pointer_cast<T>(Obj);
				})
			| std::views::filter([](const auto& Casted)
				{
					return Casted != nullptr;
				});

		std::ranges::copy(CastedView, std::back_inserter(OutObjects));

		return true;
	}

	template <typename T>
	std::vector<std::weak_ptr<T>> FindObjectsOfType()
	{
		auto CastedViews = Objects | std::views::values
			| std::views::transform([](const auto& Obj)
				{
					return std::dynamic_pointer_cast<T>(Obj);
				})
			| std::views::filter([](const auto& Casted)
				{
					return Casted != nullptr;
				});

		return { CastedViews.begin(), CastedViews.end() };
	}

	template <typename T>
	std::weak_ptr<T> FindObjectOfType()
	{
		auto It = std::ranges::find_if(Objects | std::views::values, [](const auto& Object)
			{
				return std::dynamic_pointer_cast<T>(Object) != nullptr;
			});

		if (It != std::ranges::end(Objects | std::views::values))
		{
			return std::dynamic_pointer_cast<T>(*It);
		}

		return {};
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

	std::weak_ptr<CWorldUIManager> GetUIManager() const
	{
		return UIManager;
	}

protected:
	std::unordered_multimap<std::string, std::shared_ptr<CGameObject>> Objects;
	std::vector<std::weak_ptr<CGameObject>> StartObjects;
	std::shared_ptr<CCameraManager> CameraManager;
	std::shared_ptr<CWorldAssetManager> WorldAssetManager;
	std::shared_ptr<CInput> Input;
	std::shared_ptr<CWorldCollision> Collision;
	std::shared_ptr<CWorldUIManager> UIManager;

public:
	virtual bool Init();
	virtual void Update(const float DeltaTime);
	virtual void PostUpdate(const float DeltaTime);
	virtual void Render();
	virtual void PostRender();
	virtual void RenderUI();

private:
	void Begin();

public:
	CWorld();
	virtual ~CWorld();
};


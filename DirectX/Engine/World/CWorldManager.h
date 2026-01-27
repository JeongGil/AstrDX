#pragma once

#include "../EngineInfo.h"
#include "CWorld.h"

class CWorldManager
{
public:
	/**
	 * @brief Creates a new world of the specified type and initializes it.
	 *
	 * This templated method creates a new world instance of type `T` and assigns it
	 * to either the current world or the next world, based on the `bNext` parameter.
	 * If the world creation or initialization fails, an empty weak pointer is returned.
	 * Otherwise, a weak pointer to the newly created world is returned.
	 *
	 * @tparam T The type of the world to create. Must be derived from `CWorld`.
	 * @param bNext Determines whether the new world is assigned to the next world
	 *              (`true`) or the current world (`false`). Defaults to `true`.
	 * @return A weak pointer to the newly created world of type `T`, or an empty
	 *         weak pointer if creation or initialization fails.
	 */
	template <typename T>
	std::weak_ptr<T> CreateWorld(bool bNext = true)
	{
		std::shared_ptr<CWorld>& TargetWorld = bNext ? NextWorld : World;

		TargetWorld = std::make_shared<T>();
		if (!TargetWorld || !TargetWorld->Init())
		{
			return std::weak_ptr<T>();
		}

		return std::dynamic_pointer_cast<T>(TargetWorld);
	}

public:
	std::weak_ptr<CWorld> GetWorld() const
	{
		return World;
	}

private:
	std::shared_ptr<CWorld> World;
	std::shared_ptr<CWorld> NextWorld;

public:
	/**
	 * @brief Initializes the world manager and its associated world.
	 *
	 * This method sets up the `CWorldManager` by creating a shared instance of `CWorld`
	 * and initializing it. If the world instance or its initialization fails, the method
	 * returns `false`. Otherwise, it returns `true` upon successful initialization.
	 *
	 * @return true if the world manager and its world are successfully initialized; false otherwise.
	 */
	bool Init();
	void Update(const float DeltaTime);
	void PostUpdate(const float DeltaTime);
	void Render();
	void RenderUI();
	void PostRender();

public:
	static CWorldManager* GetInst()
	{
		if (Instance == nullptr)
		{
			Instance = new CWorldManager;
		}

		return Instance;
	}

	static void DestroyInst()
	{
		SAFE_DELETE(Instance)
	}

private:
	CWorldManager();
	~CWorldManager();

	inline static CWorldManager* Instance = nullptr;
};


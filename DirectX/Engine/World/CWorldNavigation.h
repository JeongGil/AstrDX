#pragma once

#include "../EngineInfo.h"

class CThreadQueue;
class CTileMapComponent;
class CThreadNavigation;
class CWorld;

class CWorldNavigation : public std::enable_shared_from_this<CWorldNavigation>
{
	friend CWorld;

private:
	CWorldNavigation();

public:
	~CWorldNavigation();

private:
	std::weak_ptr<CWorld> World;

	std::vector<std::shared_ptr<CThreadNavigation>> Threads;
	std::shared_ptr<CThreadQueue> NavQueue;

public:
	bool Init();
	void Update(const float DeltaTime);
	void CreateNavigationThread(int Count, const std::weak_ptr<CTileMapComponent>& TileMap);
	void FindPath(const FVector2& Start, const FVector2& End);

private:
	void Begin();

public:
	std::weak_ptr<CWorld> GetWorld() const
	{
		return World;
	}

	void SetWorld(const std::weak_ptr<CWorld>& World)
	{
		this->World = World;
	}
};


#pragma once

#include "../EngineInfo.h"

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

public:
	bool Init();
	void Update(const float DeltaTime);
	void CreateNavigationThread(int Count, const std::weak_ptr<CTileMapComponent>& TileMap);	

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


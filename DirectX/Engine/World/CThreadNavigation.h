#pragma once
#include "../CThreadBase.h"

class CTileMapComponent;
class CThreadManager;

class CThreadNavigation :
    public CThreadBase
{
	friend CThreadManager;

protected:
	CThreadNavigation();

public:
	~CThreadNavigation() override;

private:
	std::weak_ptr<CTileMapComponent> TileMap;

public:
	void Exit() override;
	void Run() override;

	void SetTileMap(const std::weak_ptr<CTileMapComponent>& TileMap);
};


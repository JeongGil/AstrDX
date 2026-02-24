#pragma once
#include "../CThreadBase.h"

class CNavigation;
class CWorldNavigation;
class CTileMapComponent;
class CThreadManager;

namespace ENavigationHeader
{
	enum Type
	{
		FindPath,
		FindComplete,
		Exit
	};
}

class CThreadNavigation :
    public CThreadBase
{
	friend CThreadManager;

protected:
	CThreadNavigation();

public:
	~CThreadNavigation() override;

private:
	std::weak_ptr<CWorldNavigation> WorldNavigation;
	std::shared_ptr<CNavigation> Navigation;

public:
	void Exit() override;
	void Run() override;

	void SetWorldNavigation(const std::weak_ptr<CWorldNavigation>& Nav);
	void SetTileMap(const std::weak_ptr<CTileMapComponent>& TileMap);
};


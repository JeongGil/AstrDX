#pragma once
#include <Object/CTileMapObject.h>

class CWorld;

class CTileMapMain :
    public CTileMapObject
{
	friend CWorld;
	friend CObject;

protected:
	CTileMapMain();
	CTileMapMain(const CTileMapMain& Other) = default;
	CTileMapMain(CTileMapMain&& Other) noexcept = default;

public:
	~CTileMapMain() override;

	void Begin() override;
	bool Init() override;

	void LoadTileMap(const TCHAR* FileName, const std::string& PathName = "Root");
};


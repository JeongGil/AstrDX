#pragma once
#include <World\CWorld.h>

class CTileMapMain;
class CEditorPlayer;

class CEditorWorld :
    public CWorld
{
public:
	CEditorWorld();
	~CEditorWorld() override;

private:
	std::weak_ptr<CEditorPlayer> Player;
	std::weak_ptr<CTileMapMain> TileMap;

public:
	bool Init() override;
	void Update(const float DeltaTime) override;

private:
	void LoadAnimation2D();
	void LoadSound();
	void CreateUI();
};


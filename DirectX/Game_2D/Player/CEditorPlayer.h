#pragma once
#include <Object\CGameObject.h>

#include "../ClientInfo.h"

class CEditorWidget;
class CTileMapObject;
class CObjectMovementComponent;
class CCameraComponent;
class CWorld;

class CEditorPlayer :
    public CGameObject
{
	friend CWorld;
	friend CObject;

protected:
	CEditorPlayer();
	CEditorPlayer(const CEditorPlayer& Other) = default;
	CEditorPlayer(CEditorPlayer&& Other) noexcept = default;

public:
	~CEditorPlayer() override;

private:
	std::weak_ptr<CCameraComponent> CameraComponent;
	std::weak_ptr<CObjectMovementComponent> Movement;
	std::weak_ptr<CTileMapObject> TileMap;
	std::weak_ptr<CEditorWidget> EditorWidget;

	EEditorMode EditorMode{ EEditorMode::TileFrame };
	int EditorFrame{ 0 };
	ETileType TileType{ ETileType::Normal };

public:
	void Begin() override;
	bool Init() override;
	void Update(float DeltaTime) override;

private:
	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void LeftClick();
	void Save();
	void Load();
	void ChangeMode();
	void ChangeTileType();
	void ChangeFrame();
};


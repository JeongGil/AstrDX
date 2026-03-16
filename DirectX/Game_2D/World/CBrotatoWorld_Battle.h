#pragma once
#include <World/CWorld.h>

class CColliderBox2D;
class CCameraObject;

class CBrotatoWorld_Battle :
    public CWorld
{
public:
	CBrotatoWorld_Battle() = default;
	~CBrotatoWorld_Battle() override = default;

protected:
	int StageLevel;

	int TotalStageTime;
	float RemainStageTime;

	int ItemBoxDropCount{};

	std::weak_ptr<CCameraObject> SubCameraObj;

	int TileTheme{ 1 };
	int TileCountX{ 16 };
	int TileCountY{ 16 };

	std::array<std::weak_ptr<CColliderBox2D>, 4> EdgeColliders;

public:
	bool Init() override;
	void Update(const float DeltaTime) override;

	void FinishStage(bool bClear);

private:
void LoadAnimation2D();
void LoadSound();
void CreateUI();
void CreateTileMap();

public:
	[[nodiscard]] int GetItemBoxDropCount() const
	{
		return ItemBoxDropCount;
	}

	void SetItemBoxDropCount(const int ItemBoxDropCount)
	{
		this->ItemBoxDropCount = ItemBoxDropCount;
	}

	int GetStageLevel() const
	{
		return StageLevel;
	}
	void SetStageLevel(int NewLevel)
	{
		this->StageLevel = NewLevel;
	}

	[[nodiscard]] std::weak_ptr<CCameraObject> GetSubCameraObj() const
	{
		return SubCameraObj;
	}

	void SetTileTheme(const int InTheme)
	{
		TileTheme = InTheme;
	}

	void SetTileCount(int InCountX, int InCountY)
	{
		TileCountX = InCountX;
		TileCountY = InCountY;
	}

	[[nodiscard]] int GetTileCountX() const { return TileCountX; }
	[[nodiscard]] int GetTileCountY() const { return TileCountY; }
	[[nodiscard]] int GetTileTheme() const { return TileTheme; }
};


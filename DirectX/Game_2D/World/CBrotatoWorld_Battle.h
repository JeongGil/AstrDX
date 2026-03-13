#pragma once
#include <World/CWorld.h>

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

public:
	bool Init() override;
	void Update(const float DeltaTime) override;

	void FinishStage(bool bClear);

private:
	void LoadAnimation2D();
	void LoadSound();
	void CreateUI();

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
};


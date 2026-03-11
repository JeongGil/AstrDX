#pragma once
#include <World/CWorld.h>

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
};


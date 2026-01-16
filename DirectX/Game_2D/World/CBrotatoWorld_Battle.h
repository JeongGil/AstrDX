#pragma once
#include <World/CWorld.h>

class CBrotatoWorld_Battle :
    public CWorld
{
public:
	bool Init() override;
	void Update(const float DeltaTime) override;

	void FinishStage(bool bClear);

	int GetStageLevel() const
	{
		return StageLevel;
	}
	void SetStageLevel(int NewLevel)
	{
		this->StageLevel = NewLevel;
	}

protected:
	int StageLevel;

	int TotalStageTime;
	float RemainStageTime;

public:
	~CBrotatoWorld_Battle() override = default;
};


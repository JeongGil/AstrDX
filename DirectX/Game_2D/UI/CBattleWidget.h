#pragma once
#include <UI/CWidgetContainer.h>

class CResultWidget;
class CPlayerCharacter;
class CLootStateWidget;
class CStageStateWidget;
class CPCStateWidget;
class CWorldUIManager;

class CBattleWidget :
    public CWidgetContainer
{
	friend CWorldUIManager;

protected:
	CBattleWidget() = default;

public:
	~CBattleWidget() override = default;

private:
	std::weak_ptr<CPCStateWidget> PCStateWidget;
	std::weak_ptr<CStageStateWidget> StageStateWidget;
	std::weak_ptr<CLootStateWidget> LootStateWidget;

	std::weak_ptr<CResultWidget> BattleResultWidget;

public:
	bool Init() override;

	void SetPlayerCharacter(const std::weak_ptr<CPlayerCharacter>& PC);

	[[nodiscard]] std::weak_ptr<CResultWidget> GetBattleResultWidget() const
	{
		return BattleResultWidget;
	}

	[[nodiscard]] std::weak_ptr<CPCStateWidget> GetPcStateWidget() const
	{
		return PCStateWidget;
	}

	[[nodiscard]] std::weak_ptr<CStageStateWidget> GetStageStateWidget() const
	{
		return StageStateWidget;
	}

	[[nodiscard]] std::weak_ptr<CLootStateWidget> GetLootStateWidget() const
	{
		return LootStateWidget;
	}
};


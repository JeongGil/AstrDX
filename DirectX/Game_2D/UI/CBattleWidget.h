#pragma once
#include <UI/CWidgetContainer.h>

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

public:
	bool Init() override;

	void SetPlayerCharacter(const std::weak_ptr<CPlayerCharacter>& PC);
};


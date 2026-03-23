#pragma once
#include <UI/CWidgetContainer.h>

class CTextBlock;
class CWorldUIManager;

class CStageStateWidget :
	public CWidgetContainer
{
	friend CWorldUIManager;
	friend CWidgetContainer;

protected:
	CStageStateWidget() = default;

public:
	~CStageStateWidget() override = default;

private:
	std::weak_ptr<CTextBlock> StageLevel;
	std::weak_ptr<CTextBlock> RemainTime;

public:
	bool Init() override;
	void Update(const float DeltaTime) override;
};


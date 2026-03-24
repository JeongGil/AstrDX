#pragma once
#include <UI/CWidgetContainer.h>

class CImage;
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
	std::weak_ptr<CTextBlock> StageResult;

	std::weak_ptr<CImage> ResultBG;

public:
	bool Init() override;
	void Update(const float DeltaTime) override;

	void SetEnableStageResult(bool bEnable) const;
	void SetStageResultText(bool bClear) const;
};


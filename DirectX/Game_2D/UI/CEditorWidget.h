#pragma once
#include <UI\CWidgetContainer.h>

#include "../ClientInfo.h"

class CTextBlock;
class CWorldUIManager;

class CEditorWidget :
    public CWidgetContainer
{
	friend CWorldUIManager;

protected:
	CEditorWidget();

public:
	~CEditorWidget() override;

private:
	std::shared_ptr<CTextBlock> EditorMode;
	std::shared_ptr<CTextBlock> TileType;
	std::shared_ptr<CTextBlock> TileFrame;

public:
	bool Init() override;
	void Update(const float DeltaTime) override;
	void Render() override;

	void SetEditorMode(EEditorMode Mode);
	void SetTileFrame(int Frame);
	void SetTileType(ETileType TileType);
};


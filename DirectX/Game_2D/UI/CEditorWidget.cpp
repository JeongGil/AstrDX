#include "CEditorWidget.h"

#include <UI/CTextBlock.h>

CEditorWidget::CEditorWidget()
{
}

CEditorWidget::~CEditorWidget()
{
}

bool CEditorWidget::Init()
{
	if (!CWidgetContainer::Init())
	{
		return false;
	}

	EditorMode = CreateWidget<CTextBlock>("EditorMode").lock();

	EditorMode->SetText(TEXT("EditorMode : Frame"));
	EditorMode->SetTextColor(255, 0, 0, 255);
	EditorMode->SetPos(20.f, 20.f);
	EditorMode->SetSize(300.f, 100.f);
	EditorMode->SetFontSize(30.f);
	EditorMode->EnableShadow(true);
	EditorMode->SetShadowOffset(3.f, 3.f);
	EditorMode->SetShadowTextColor(128, 128, 128, 255);

	TileType = CreateWidget<CTextBlock>("TileType").lock();

	TileType->SetText(TEXT("TileType : Movable"));
	TileType->SetTextColor(255, 0, 0, 255);
	TileType->SetPos(420.f, 20.f);
	TileType->SetSize(300.f, 100.f);
	TileType->SetFontSize(30.f);
	TileType->EnableShadow(true);
	TileType->SetShadowOffset(3.f, 3.f);
	TileType->SetShadowTextColor(128, 128, 128, 255);

	TileFrame = CreateWidget<CTextBlock>("TileFrame").lock();

	TileFrame->SetText(TEXT("TileFrame : 0"));
	TileFrame->SetTextColor(255, 0, 0, 255);
	TileFrame->SetPos(720.f, 20.f);
	TileFrame->SetSize(300.f, 100.f);
	TileFrame->SetFontSize(30.f);
	TileFrame->EnableShadow(true);
	TileFrame->SetShadowOffset(3.f, 3.f);
	TileFrame->SetShadowTextColor(128, 128, 128, 255);

	return true;
}

void CEditorWidget::Update(const float DeltaTime)
{
	CWidgetContainer::Update(DeltaTime);
}

void CEditorWidget::Render()
{
	CWidgetContainer::Render();
}

void CEditorWidget::SetEditorMode(EEditorMode Mode)
{
	switch (Mode)
	{
		case EEditorMode::TileType:
			EditorMode->SetText(TEXT("EditorMode : Type"));
			break;
		case EEditorMode::TileFrame:
			EditorMode->SetText(TEXT("EditorMode : Frame"));
			break;
	}
}

void CEditorWidget::SetTileFrame(int Frame)
{
	TCHAR Text[256] = {};

	wsprintf(Text, TEXT("TileFrame : %d"), Frame);

	TileFrame->SetText(Text);
}

void CEditorWidget::SetTileType(ETileType Type)
{
	switch (Type)
	{
		case ETileType::Normal:
			TileType->SetText(TEXT("TileType : Movable"));
			break;
		case ETileType::UnableToMove:
			TileType->SetText(TEXT("TileType : UnMovable"));
			break;
		case ETileType::End:
			break;
	}
}

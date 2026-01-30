#pragma once
#include "CWidget.h"

class CTextBlock;
class CWorldUIManager;
class CWidgetContainer;
class CTexture;

class CTitleBar :
	public CWidget
{
	friend CWorldUIManager;
	friend CWidgetContainer;
	friend CWidget;

protected:
	CTitleBar();
	CTitleBar(const CTitleBar& other);

public:
	~CTitleBar() override;

protected:
	FUIBrush Brush;
	std::shared_ptr<CTextBlock> Child;
	std::weak_ptr<CWidget> UpdateWidget;

public:
	void SetUpdateWidget(const std::weak_ptr<CWidget>& Widget);
	void SetText(const TCHAR* Text);
	void SetZOrder(int ZOrder) override;
	void ReplaceZOrder() override;

	bool SetTexture(const std::weak_ptr<CTexture>& Texture);
	bool SetTexture(const std::string& Key);
	bool SetTexture(const std::string& Key, const TCHAR* FileName, const std::string& PathName = "Texture");
	void SetTint(const FColor& Tint);
	void SetTint(float r, float g, float b, float a);
	void SetBrushAnimation(bool Animation);
	void AddBrushFrame(const FVector2& Start, const FVector2& Size);
	void AddBrushFrame(float StartX, float StartY, float SizeX, float SizeY);
	void SetCurrentFrame(int Frame);
	void SetAnimationPlayTime(float PlayTime);
	void SetAnimationPlayRate(float PlayRate);

	bool Init() override;
	void Update(const float DeltaTime) override;
	void Render() override;
	bool CollideMouse(std::weak_ptr<CWidget>& Result, const FVector2& MousePos) override;
	void MouseHovered() override;
	void MouseUnHovered() override;
	bool MouseDragStart(const FVector2& MousePos, std::shared_ptr<CWidget>& DragOperator) override;
	bool MouseDrag(const FVector2& MousePos, const FVector2& MouseMove) override;
	bool MouseDragEnd(const FVector2& MousePos) override;
	CTitleBar* Clone() const override;
	void SetParentAll() override;
};


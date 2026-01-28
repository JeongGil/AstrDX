#pragma once
#include "CWidget.h"

class CWorldUIManager;
class CWidgetContainer;
class CTexture;

class CImage :
	public CWidget
{
	friend CWorldUIManager;
	friend CWidgetContainer;
	friend CWidget;

protected:
	CImage();

public:
	~CImage() override;

protected:
	FUIBrush Brush;

public:
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
};


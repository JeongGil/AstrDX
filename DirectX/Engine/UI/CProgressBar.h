#pragma once
#include "CWidget.h"

class CTextBlock;

namespace EProgressBarImageType
{
	enum Type
	{
		Back,
		Fill,
		End
	};
}

enum class EProgressBarAnchor
{
	Left,
	Bottom,
};

class CWorldUIManager;
class CWidgetContainer;
class CTexture;

class CProgressBar :
	public CWidget
{
	friend CWorldUIManager;
	friend CWidgetContainer;
	friend CWidget;

protected:
	CProgressBar();
	CProgressBar(const CProgressBar& other);

public:
	~CProgressBar() override;

protected:
	FUIBrush Brush[EProgressBarImageType::End];
	EProgressBarAnchor BarAnchor = EProgressBarAnchor::Left;
	float Ratio = 1.f;
	float MinValue = 0.f;
	float MaxValue = 1.f;

	std::shared_ptr<CTextBlock> Child;
	bool bTextEnable = false;

public:
	void SetTextEnable(bool bEnable);
	void SetMin(float Min);
	void SetMax(float Max);
	void SetRatio(float Ratio);
	void SetValue(float Value);
	void SetBarAnchor(EProgressBarAnchor Anchor);

	bool SetTexture(EProgressBarImageType::Type State, const std::weak_ptr<CTexture>& Texture);
	bool SetTexture(EProgressBarImageType::Type State, const std::string& Key);
	bool SetTexture(EProgressBarImageType::Type State, const std::string& Key, const TCHAR* FileName, const std::string& PathName = "Texture");
	void SetTint(EProgressBarImageType::Type State, const FColor& Tint);
	void SetTint(EProgressBarImageType::Type State, float r, float g, float b, float a);
	void SetBrushAnimation(EProgressBarImageType::Type State, bool Animation);
	void AddBrushFrame(EProgressBarImageType::Type State, const FVector2& Start, const FVector2& Size);
	void AddBrushFrame(EProgressBarImageType::Type State, float StartX, float StartY, float SizeX, float SizeY);
	void SetCurrentFrame(EProgressBarImageType::Type State, int Frame);
	void SetAnimationPlayTime(EProgressBarImageType::Type State, float PlayTime);
	void SetAnimationPlayRate(EProgressBarImageType::Type State, float PlayRate);

	bool Init() override;
	void Update(const float DeltaTime) override;
	void Render() override;
	bool CollideMouse(std::weak_ptr<CWidget>& Result, const FVector2& MousePos) override;
	void MouseHovered() override;
	void MouseUnHovered() override;
	CProgressBar* Clone() const override;
	void SetParentAll() override;
	void SetOpacityAll(float Opacity) override;
};


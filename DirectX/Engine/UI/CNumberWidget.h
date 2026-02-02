#pragma once
#include "CWidget.h"

class CWorldUIManager;
class CWidgetContainer;
class CTexture;

class CNumberWidget :
	public CWidget
{
	friend CWorldUIManager;
	friend CWidgetContainer;
	friend CWidget;

protected:
	CNumberWidget();
	CNumberWidget(const CNumberWidget& other);

public:
	~CNumberWidget() override;

protected:
	FUIBrush Brush;
	float Number = 0.f;
	int DecimalCount = 2;
	FVector NumberSize;
	std::vector<int> Numbers;
	std::vector<int> DecimalNumbers;
	FVector3 NumberRenderPos;

public:
	void SetDecimalCount(int Count)
	{
		DecimalCount = Count;
	}

	void SetNumber(float Number)
	{
		this->Number = Number;
	}

	void AddNumber(float Number)
	{
		this->Number += Number;
	}

	void SetNumberSize(const FVector& Size)
	{
		NumberSize = Size;
	}

	void SetNumberSize(float x, float y)
	{
		NumberSize.x = x;
		NumberSize.y = y;
	}

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
	CNumberWidget* Clone() const override;

private:
	void RenderNumber();
};


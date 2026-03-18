#pragma once
#include <UI/CWidget.h>

class CTextBlock;
class CWorldUIManager;
class CWidgetContainer;
class CTexture;

namespace EBrotatoProgressBarImageType
{
	enum Type
	{
		Frame,
		Back,
		Fill,
		End
	};
}

enum class EBrotatoProgressBarType
{
	HP,
	EXP
};

class CBrotatoProgressBar :
	public CWidget
{
	friend CWorldUIManager;
	friend CWidgetContainer;
	friend CWidget;

protected:
	CBrotatoProgressBar();
	CBrotatoProgressBar(const CBrotatoProgressBar& Other);

public:
	~CBrotatoProgressBar() override;

private:
	FUIBrush Brush[EBrotatoProgressBarImageType::End];
	float Ratio = 1.f;

	float CurrentValue = 0.f;
	float MaxValue = 1.f;
	int CurrentLevel = 1;

	EBrotatoProgressBarType BarType = EBrotatoProgressBarType::HP;

	std::shared_ptr<CTextBlock> Text;
	float TextPaddingX = 8.f;

	float FramePadding = 8.f;
	float FillPadding = 4.f;

private:
	void UpdateText();
	void UpdateTextLayout();

public:
	void SetBarType(EBrotatoProgressBarType Type);
	void SetRatio(float InRatio);
	void SetHPValue(float InCurrentHP, float InMaxHP);
	void SetEXPValue(float InCurrentEXP, float InNeedEXP, int InCurrentLevel);
	void SetTextColor(const FColor& Color);
	void SetTextColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	void SetTextPaddingX(float InPaddingX);

	bool SetTexture(EBrotatoProgressBarImageType::Type State, const std::weak_ptr<CTexture>& Texture);
	bool SetTexture(EBrotatoProgressBarImageType::Type State, const std::string& Key);
	bool SetTexture(EBrotatoProgressBarImageType::Type State, const std::string& Key, const TCHAR* FileName,
		const std::string& PathName = "Texture");
	void SetTint(EBrotatoProgressBarImageType::Type State, const FColor& Tint);
	void SetTint(EBrotatoProgressBarImageType::Type State, float r, float g, float b, float a);
	void SetBrushAnimation(EBrotatoProgressBarImageType::Type State, bool bAnimation);
	void AddBrushFrame(EBrotatoProgressBarImageType::Type State, const FVector2& Start, const FVector2& Size);
	void AddBrushFrame(EBrotatoProgressBarImageType::Type State, float StartX, float StartY, float SizeX, float SizeY);
	void SetCurrentFrame(EBrotatoProgressBarImageType::Type State, int Frame);
	void SetAnimationPlayTime(EBrotatoProgressBarImageType::Type State, float PlayTime);
	void SetAnimationPlayRate(EBrotatoProgressBarImageType::Type State, float PlayRate);

	void SetSize(const FVector& InSize) override;
	void SetSize(float x, float y) override;
	void SetParentAll() override;
	void SetOpacityAll(float Opacity) override;

	bool Init() override;
	void Update(const float DeltaTime) override;
	void Render() override;
	bool CollideMouse(std::weak_ptr<CWidget>& Result, const FVector2& MousePos) override;
	void MouseHovered() override;
	void MouseUnHovered() override;
	CBrotatoProgressBar* Clone() const override;
};


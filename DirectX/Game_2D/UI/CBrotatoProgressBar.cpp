#include "CBrotatoProgressBar.h"

#include <UI/CTextBlock.h>
#include <World/CWorld.h>
#include "../Strings.h"

CBrotatoProgressBar::CBrotatoProgressBar()
{
}

CBrotatoProgressBar::CBrotatoProgressBar(const CBrotatoProgressBar& Other) :
	CWidget(Other)
{
	std::ranges::copy(Other.Brush, Brush);

	Ratio = Other.Ratio;
	CurrentValue = Other.CurrentValue;
	MaxValue = Other.MaxValue;
	CurrentLevel = Other.CurrentLevel;
	BarType = Other.BarType;
	TextPaddingX = Other.TextPaddingX;
	FramePadding = Other.FramePadding;
	FillPadding = Other.FillPadding;

	if (Other.Text)
	{
		Text.reset(Other.Text->Clone());
	}
}

CBrotatoProgressBar::~CBrotatoProgressBar()
{
}

void CBrotatoProgressBar::UpdateText()
{
	if (!Text)
	{
		return;
	}

	if (BarType == EBrotatoProgressBarType::HP)
	{
		Text->SetText(static_cast<int>(CurrentValue));
		Text->AddText(TEXT(" / "));
		Text->AddText(static_cast<int>(MaxValue));
	}
	else
	{
		Text->SetText(TEXT("LV."));
		Text->AddText(CurrentLevel);
	}
}

void CBrotatoProgressBar::UpdateTextLayout()
{
	if (!Text)
	{
		return;
	}

	if (BarType == EBrotatoProgressBarType::HP)
	{
		Text->SetPos(0.f, 0.f);
		Text->SetSize(Size.x, Size.y);
		Text->SetAlignH(ETextAlignH::Center);
	}
	else
	{
		Text->SetPos(TextPaddingX, 0.f);
		Text->SetSize((std::max)(0.f, Size.x - (TextPaddingX * 2.f)), Size.y);
		Text->SetAlignH(ETextAlignH::Right);
	}

	Text->SetAlignV(ETextAlignV::Middle);
}

void CBrotatoProgressBar::SetBarType(EBrotatoProgressBarType Type)
{
	BarType = Type;

	if (BarType == EBrotatoProgressBarType::HP)
	{
		SetTint(EBrotatoProgressBarImageType::Fill, FColor::Red);
	}
	else
	{
		SetTint(EBrotatoProgressBarImageType::Fill, FColor::Green);
	}

	UpdateTextLayout();
	UpdateText();
}

void CBrotatoProgressBar::SetRatio(float InRatio)
{
	Ratio = std::clamp(InRatio, 0.f, 1.f);
}

void CBrotatoProgressBar::SetHPValue(float InCurrentHP, float InMaxHP)
{
	BarType = EBrotatoProgressBarType::HP;
	CurrentValue = (std::max)(0.f, InCurrentHP);
	MaxValue = (std::max)(1.f, InMaxHP);

	SetRatio(CurrentValue / MaxValue);
	UpdateTextLayout();
	UpdateText();
}

void CBrotatoProgressBar::SetEXPValue(float InCurrentEXP, float InNeedEXP, int InCurrentLevel)
{
	BarType = EBrotatoProgressBarType::EXP;
	CurrentLevel = (std::max)(1, InCurrentLevel);

	const float NeedEXP = (std::max)(1.f, InNeedEXP);
	const float CurrentEXP = std::clamp(InCurrentEXP, 0.f, NeedEXP);

	SetRatio(CurrentEXP / NeedEXP);
	UpdateTextLayout();
	UpdateText();
}

void CBrotatoProgressBar::SetTextColor(const FColor& Color)
{
	if (Text)
	{
		Text->SetTextColor(Color);
	}
}

void CBrotatoProgressBar::SetTextColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	SetTextColor(FColor(r, g, b, a));
}

void CBrotatoProgressBar::SetTextPaddingX(float InPaddingX)
{
	TextPaddingX = (std::max)(0.f, InPaddingX);
	UpdateTextLayout();
}

bool CBrotatoProgressBar::SetTexture(EBrotatoProgressBarImageType::Type State, const std::weak_ptr<CTexture>& Texture)
{
	Brush[State].Texture = Texture;

	return true;
}

bool CBrotatoProgressBar::SetTexture(EBrotatoProgressBarImageType::Type State, const std::string& Key)
{
	if (auto World = this->World.lock())
	{
		if (auto AssetMgr = World->GetWorldAssetManager().lock())
		{
			if (auto Texture = AssetMgr->FindTexture(Key).lock())
			{
				Brush[State].Texture = Texture;
				return true;
			}
		}
	}

	return false;
}

bool CBrotatoProgressBar::SetTexture(EBrotatoProgressBarImageType::Type State, const std::string& Key,
	const TCHAR* FileName, const std::string& PathName)
{
	if (auto World = this->World.lock())
	{
		if (auto AssetMgr = World->GetWorldAssetManager().lock())
		{
			if (!AssetMgr->LoadTexture(Key, FileName, PathName))
			{
				return false;
			}

			if (auto Texture = AssetMgr->FindTexture(Key).lock())
			{
				Brush[State].Texture = Texture;
				return true;
			}
		}
	}

	return false;
}

void CBrotatoProgressBar::SetTint(EBrotatoProgressBarImageType::Type State, const FColor& Tint)
{
	Brush[State].Tint = Tint;
}

void CBrotatoProgressBar::SetTint(EBrotatoProgressBarImageType::Type State, float r, float g, float b, float a)
{
	SetTint(State, FColor(r, g, b, a));
}

void CBrotatoProgressBar::SetBrushAnimation(EBrotatoProgressBarImageType::Type State, bool bAnimation)
{
	Brush[State].AnimationEnable = bAnimation;
}

void CBrotatoProgressBar::AddBrushFrame(EBrotatoProgressBarImageType::Type State, const FVector2& Start,
	const FVector2& InSize)
{
	auto& ImageBrush = Brush[State];
	ImageBrush.AnimationFrames.emplace_back(Start, InSize);
	ImageBrush.FrameTime = ImageBrush.PlayTime / ImageBrush.AnimationFrames.size();
}

void CBrotatoProgressBar::AddBrushFrame(EBrotatoProgressBarImageType::Type State, float StartX, float StartY,
	float SizeX, float SizeY)
{
	AddBrushFrame(State, FVector2(StartX, StartY), FVector2(SizeX, SizeY));
}

void CBrotatoProgressBar::SetCurrentFrame(EBrotatoProgressBarImageType::Type State, int Frame)
{
	Brush[State].Frame = Frame;
}

void CBrotatoProgressBar::SetAnimationPlayTime(EBrotatoProgressBarImageType::Type State, float PlayTime)
{
	auto& ImageBrush = Brush[State];
	ImageBrush.PlayTime = PlayTime;

	if (!ImageBrush.AnimationFrames.empty())
	{
		ImageBrush.FrameTime = ImageBrush.PlayTime / ImageBrush.AnimationFrames.size();
	}
	else
	{
		ImageBrush.FrameTime = FLT_MAX;
	}
}

void CBrotatoProgressBar::SetAnimationPlayRate(EBrotatoProgressBarImageType::Type State, float PlayRate)
{
	Brush[State].PlayRate = PlayRate;
}

void CBrotatoProgressBar::SetSize(const FVector& InSize)
{
	CWidget::SetSize(InSize);
	UpdateTextLayout();
}

void CBrotatoProgressBar::SetSize(float x, float y)
{
	CWidget::SetSize(x, y);
	UpdateTextLayout();
}

void CBrotatoProgressBar::SetParentAll()
{
	if (Text)
	{
		Text->SetParent(std::dynamic_pointer_cast<CWidget>(shared_from_this()));
		Text->SetParentAll();
	}
}

void CBrotatoProgressBar::SetOpacityAll(float Opacity)
{
	CWidget::SetOpacityAll(Opacity);

	if (Text)
	{
		Text->SetOpacityAll(Opacity);
	}
}

bool CBrotatoProgressBar::Init()
{
	if (!CWidget::Init())
	{
		return false;
	}

	SetTexture(EBrotatoProgressBarImageType::Frame, "LifeBarFrame", TEXT("ui\\hud\\ui_lifebar_frame.png"), Key::Path::Brotato);
	SetTexture(EBrotatoProgressBarImageType::Back, "LifeBarBG", TEXT("ui\\hud\\ui_lifebar_bg.png"), Key::Path::Brotato);
	SetTexture(EBrotatoProgressBarImageType::Fill, "LifeBarFill", TEXT("ui\\hud\\ui_lifebar_fill.png"), Key::Path::Brotato);

	SetTint(EBrotatoProgressBarImageType::Frame, FColor::White);
	SetTint(EBrotatoProgressBarImageType::Back, FColor::White);

	if (BarType == EBrotatoProgressBarType::HP)
	{
		SetTint(EBrotatoProgressBarImageType::Fill, FColor::Red);
	}
	else
	{
		SetTint(EBrotatoProgressBarImageType::Fill, FColor::Green);
	}

	Text = CreateStaticWidget<CTextBlock>("BrotatoBarText", World);
	if (Text)
	{
		Text->SetParent(std::dynamic_pointer_cast<CWidget>(shared_from_this()));
		Text->SetUIManager(UIManager);
		Text->SetTextColor(255, 255, 255, 255);
		Text->SetFontSize(16.f);
		UpdateTextLayout();
		UpdateText();
	}

	return true;
}

void CBrotatoProgressBar::Update(const float DeltaTime)
{
	CWidget::Update(DeltaTime);

	Brush[EBrotatoProgressBarImageType::Frame].PlayAnimation(DeltaTime);
	Brush[EBrotatoProgressBarImageType::Back].PlayAnimation(DeltaTime);
	Brush[EBrotatoProgressBarImageType::Fill].PlayAnimation(DeltaTime);
}

void CBrotatoProgressBar::Render()
{
	CWidget::Render();

	const float TexW = 320.f;
	const float TexH = 48.f;

	const float ScaleX = Size.x / TexW;
	const float ScaleY = Size.y / TexH;

	const float InnerOffsetX = FramePadding * ScaleX;
	const float InnerOffsetY = FramePadding * ScaleY;
	const float InnerW = Size.x - InnerOffsetX * 2.f;
	const float InnerH = Size.y - InnerOffsetY * 2.f;

	FVector InnerPos = RenderPos;
	InnerPos.x += InnerOffsetX;
	InnerPos.y += InnerOffsetY;

	FVector3 InnerSize(InnerW, InnerH, 0.f);

	FVector2 BgFillStart(FillPadding, FillPadding);
	FVector3 BgFillSize(TexW - FillPadding * 2.f, TexH - FillPadding * 2.f, 0.f);

	RenderBrush(Brush[EBrotatoProgressBarImageType::Back], InnerPos, InnerSize, BgFillStart, BgFillSize);

	const float FillW = InnerW * Ratio;
	const float FillTexW = (TexW - FillPadding * 2.f) * Ratio;

	FVector3 FillRenderSize(FillW, InnerH, 0.f);
	FVector3 FillFrameSize(FillTexW, TexH - FillPadding * 2.f, 0.f);

	RenderBrush(Brush[EBrotatoProgressBarImageType::Fill], InnerPos, FillRenderSize, BgFillStart, FillFrameSize);

	RenderBrush(Brush[EBrotatoProgressBarImageType::Frame], RenderPos, Size);

	if (Text)
	{
		Text->Render();
	}
}

bool CBrotatoProgressBar::CollideMouse(std::weak_ptr<CWidget>& Result, const FVector2& MousePos)
{
	if (!CWidget::CollideMouse(Result, MousePos))
	{
		return false;
	}

	return true;
}

void CBrotatoProgressBar::MouseHovered()
{
	CWidget::MouseHovered();
}

void CBrotatoProgressBar::MouseUnHovered()
{
	CWidget::MouseUnHovered();
}

CBrotatoProgressBar* CBrotatoProgressBar::Clone() const
{
	return new CBrotatoProgressBar(*this);
}

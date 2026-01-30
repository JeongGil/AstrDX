#include "CProgressBar.h"

#include "CTextBlock.h"
#include "../World/CWorld.h"

CProgressBar::CProgressBar()
{
}

CProgressBar::CProgressBar(const CProgressBar& other) :
	CWidget(other)
{
	std::ranges::copy(other.Brush, Brush);

	BarAnchor = other.BarAnchor;
	Ratio = other.Ratio;
	MinValue = other.MinValue;
	MaxValue = other.MaxValue;
	bTextEnable = other.bTextEnable;

	if (other.Child)
	{
		Child.reset(other.Child->Clone());
	}
}

CProgressBar* CProgressBar::Clone() const
{
	return new CProgressBar(*this);
}

void CProgressBar::SetParentAll()
{
	if (Child)
	{
		Child->SetParent(std::dynamic_pointer_cast<CWidget>(shared_from_this()));

		Child->SetParentAll();
	}
}

void CProgressBar::SetOpacityAll(float Opacity)
{
	CWidget::SetOpacityAll(Opacity);

	if (Child)
	{
		Child->SetOpacityAll(Opacity);
	}
}

CProgressBar::~CProgressBar()
{
}

void CProgressBar::SetTextEnable(bool bEnable)
{
	bTextEnable = bEnable;

	if (bEnable)
	{
		Child = CreateStaticWidget<CTextBlock>("Text", World);

		Child->SetParent(std::dynamic_pointer_cast<CWidget>(shared_from_this()));
		Child->SetSize(Size);
		Child->SetUIManager(UIManager);

		Child->SetText(MinValue);
		Child->AddText(TEXT(" / "));
		Child->AddText(MaxValue);
		Child->SetTextColor(255, 255, 0, 255);
	}
	else
	{
		if (Child)
		{
			Child.reset();
		}
	}
}

void CProgressBar::SetMin(float Min)
{
	if (Min >= MaxValue)
	{
		return;
	}

	MinValue = Min;

	if (Child)
	{
		Child->SetText(MinValue);
		Child->AddText(TEXT(" / "));
		Child->AddText(MaxValue);
	}
}

void CProgressBar::SetMax(float Max)
{
	if (Max <= MinValue)
	{
		return;
	}

	MaxValue = Max;

	if (Child)
	{
		Child->SetText(MinValue);
		Child->AddText(TEXT(" / "));
		Child->AddText(MaxValue);
	}
}

void CProgressBar::SetRatio(float Ratio)
{
	this->Ratio = std::clamp(Ratio, 0.f, 1.f);
}

void CProgressBar::SetValue(float Value)
{
	assert(MinValue != MaxValue);

	Ratio = (Value - MinValue) / (MaxValue - MinValue);
}

void CProgressBar::SetBarAnchor(EProgressBarAnchor Anchor)
{
	this->BarAnchor = Anchor;
}

bool CProgressBar::SetTexture(EProgressBarImageType::Type State, const std::weak_ptr<CTexture>& Texture)
{
	Brush[State].Texture = Texture;

	return true;
}

bool CProgressBar::SetTexture(EProgressBarImageType::Type State, const std::string& Key)
{
	auto World = this->World.lock();
	auto AssetMgr = World->GetWorldAssetManager().lock();

	auto Texture = AssetMgr->FindTexture(Key);

	return SetTexture(State, Texture);
}

bool CProgressBar::SetTexture(EProgressBarImageType::Type State, const std::string& Key, const TCHAR* FileName,
	const std::string& PathName)
{
	auto World = this->World.lock();
	auto AssetMgr = World->GetWorldAssetManager().lock();

	if (!AssetMgr->LoadTexture(Key, FileName, PathName))
	{
		return false;
	}

	auto Texture = AssetMgr->FindTexture(Key);

	return SetTexture(State, Texture);
}

void CProgressBar::SetTint(EProgressBarImageType::Type State, const FColor& Tint)
{
	Brush[State].Tint = Tint;
}

void CProgressBar::SetTint(EProgressBarImageType::Type State, float r, float g, float b, float a)
{
	SetTint(State, FColor(r, g, b, a));
}

void CProgressBar::SetBrushAnimation(EProgressBarImageType::Type State, bool Animation)
{
	Brush[State].AnimationEnable = Animation;
}

void CProgressBar::AddBrushFrame(EProgressBarImageType::Type State, const FVector2& Start, const FVector2& Size)
{
	auto& Brush = this->Brush[State];

	Brush.AnimationFrames.emplace_back(Start, Size);

	Brush.FrameTime = Brush.PlayTime / Brush.AnimationFrames.size();
}

void CProgressBar::AddBrushFrame(EProgressBarImageType::Type State, float StartX, float StartY, float SizeX,
	float SizeY)
{
	AddBrushFrame(State, FVector2(StartX, StartY), FVector2(SizeX, SizeY));
}

void CProgressBar::SetCurrentFrame(EProgressBarImageType::Type State, int Frame)
{
	Brush[State].Frame = Frame;
}

void CProgressBar::SetAnimationPlayTime(EProgressBarImageType::Type State, float PlayTime)
{
	auto& Brush = this->Brush[State];

	Brush.PlayTime = PlayTime;

	Brush.FrameTime = Brush.PlayTime / Brush.AnimationFrames.size();
}

void CProgressBar::SetAnimationPlayRate(EProgressBarImageType::Type State, float PlayRate)
{
	auto& Brush = this->Brush[State];

	Brush.PlayRate = PlayRate;

	Brush.FrameTime = Brush.PlayTime / Brush.AnimationFrames.size();
}

bool CProgressBar::Init()
{
	if (!CWidget::Init())
	{
		return false;
	}

	SetTint(EProgressBarImageType::Back, FColor::Transparent);

	return true;
}

void CProgressBar::Update(const float DeltaTime)
{
	CWidget::Update(DeltaTime);
}

void CProgressBar::Render()
{
	CWidget::Render();

	// Back
	RenderBrush(Brush[EProgressBarImageType::Back], RenderPos, Size);

	FVector Size = this->Size;
	FVector Pos = RenderPos;

	switch (BarAnchor)
	{
	case EProgressBarAnchor::Left:
		Size.x *= Ratio;
		break;
	case EProgressBarAnchor::Bottom:
		Size.y *= Ratio;
		Pos.y += Size.y * (1.f - Ratio);
		break;
	}

	// Fill
	RenderBrush(Brush[EProgressBarImageType::Fill], Pos, Size);

	if (bTextEnable)
	{
		Child->Render();
	}
}

bool CProgressBar::CollideMouse(std::weak_ptr<CWidget>& Result, const FVector2& MousePos)
{
	if (!CWidget::CollideMouse(Result, MousePos))
	{
		return false;
	}

	return true;
}

void CProgressBar::MouseHovered()
{
	CWidget::MouseHovered();
}

void CProgressBar::MouseUnHovered()
{
	CWidget::MouseUnHovered();
}

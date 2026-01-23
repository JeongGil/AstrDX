#include "CButton.h"

#include "../World/CWorld.h"

bool CButton::SetTexture(EButtonState::Type State, const std::weak_ptr<CTexture>& Texture)
{
	Brushes[State].Texture = Texture;

	return true;
}

bool CButton::SetTexture(EButtonState::Type State, const std::string& Key)
{
	if (auto World = this->World.lock())
	{
		if (auto AssetMgr = World->GetWorldAssetManager().lock())
		{
			if (auto Texture = AssetMgr->FindTexture(Key).lock())
			{
				Brushes[State].Texture = Texture;
				return true;
			}
		}
	}

	return false;
}

bool CButton::SetTexture(EButtonState::Type State, const std::string& Key, const TCHAR* FileName,
	const std::string& PathName)
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
				Brushes[State].Texture = Texture;
				return true;
			}
		}
	}

	return false;
}

void CButton::SetTint(EButtonState::Type State, const FColor& Tint)
{
	Brushes[State].Tint = Tint;
}

void CButton::SetTint(EButtonState::Type State, float r, float g, float b, float a)
{
	Brushes[State].Tint = FColor(r, g, b, a);
}

void CButton::SetBrushAnimation(EButtonState::Type State, bool Animation)
{
	Brushes[State].AnimationEnable = Animation;
}

void CButton::AddBrushFrame(EButtonState::Type State, const FVector2& Start, const FVector2& Size)
{
	auto& Brush = Brushes[State];
	Brush.AnimationFrames.emplace_back(Start, Size);

	Brush.FrameTime = Brush.PlayTime / Brush.AnimationFrames.size();
}

void CButton::AddBrushFrame(EButtonState::Type State, float StartX, float StartY, float SizeX, float SizeY)
{
	auto& Brush = Brushes[State];
	Brush.AnimationFrames.emplace_back(StartX, StartY, SizeX, SizeY);

	Brush.FrameTime = Brush.PlayTime / Brush.AnimationFrames.size();
}

void CButton::SetCurrentFrame(EButtonState::Type State, int Frame)
{
	Brushes[State].Frame = Frame;
}

void CButton::SetAnimationPlayTime(EButtonState::Type State, float PlayTime)
{
	auto& Brush = Brushes[State];
	Brush.PlayTime = PlayTime;

	if (!Brush.AnimationFrames.empty())
	{
		Brush.FrameTime = Brush.PlayTime / Brush.AnimationFrames.size();
	}
	else
	{
		Brush.FrameTime = FLT_MAX;
	}
}

void CButton::SetAnimationPlayRate(EButtonState::Type State, float PlayRate)
{
	Brushes[State].PlayRate = PlayRate;
}

bool CButton::Init()
{
	if (!CWidget::Init())
	{
		return false;
	}

	return true;
}

void CButton::Update(const float DeltaTime)
{
	CWidget::Update(DeltaTime);

	if (State == EButtonState::Disable)
	{
		return;
	}
}

void CButton::Render()
{
	CWidget::Render();
}

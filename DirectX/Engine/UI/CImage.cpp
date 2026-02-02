#include "CImage.h"

#include "../World/CWorld.h"

CImage::CImage()
{
}

CImage::CImage(const CImage& other) :
	CWidget(other)
{
	Brush = other.Brush;
}

CImage* CImage::Clone() const
{
	return new CImage(*this);
}

CImage::~CImage()
{
}

bool CImage::SetTexture(const std::weak_ptr<CTexture>& Texture)
{
	Brush.Texture = Texture;

	return true;
}

bool CImage::SetTexture(const std::string& Key)
{
	auto World = this->World.lock();
	auto AssetMgr = World->GetWorldAssetManager().lock();

	auto Texture = AssetMgr->FindTexture(Key);

	return SetTexture(Texture);
}

bool CImage::SetTexture(const std::string& Key, const TCHAR* FileName, const std::string& PathName)
{
	auto World = this->World.lock();
	auto AssetMgr = World->GetWorldAssetManager().lock();

	if (!AssetMgr->LoadTexture(Key, FileName, PathName))
	{
		return false;
	}

	auto Texture = AssetMgr->FindTexture(Key);

	return SetTexture(Texture);
}

void CImage::SetTint(const FColor& Tint)
{
	Brush.Tint = Tint;
}

void CImage::SetTint(float r, float g, float b, float a)
{
	SetTint(FColor(r, g, b, a));
}

void CImage::SetBrushAnimation(bool Animation)
{
	Brush.AnimationEnable = Animation;
}

void CImage::AddBrushFrame(const FVector2& Start, const FVector2& Size)
{
	Brush.AnimationFrames.emplace_back(Start, Size);

	Brush.FrameTime = Brush.PlayTime / Brush.AnimationFrames.size();
}

void CImage::AddBrushFrame(float StartX, float StartY, float SizeX, float SizeY)
{
	AddBrushFrame(FVector2(StartX, StartY), FVector2(SizeX, SizeY));
}

void CImage::SetCurrentFrame(int Frame)
{
	Brush.Frame = Frame;
}

void CImage::SetAnimationPlayTime(float PlayTime)
{
	Brush.PlayTime = PlayTime;

	Brush.FrameTime = Brush.PlayTime / Brush.AnimationFrames.size();
}

void CImage::SetAnimationPlayRate(float PlayRate)
{
	Brush.PlayRate = PlayRate;

	Brush.FrameTime = Brush.PlayTime / Brush.AnimationFrames.size();
}

bool CImage::Init()
{
	if (!CWidget::Init())
	{
		return false;
	}

	return true;
}

void CImage::Update(const float DeltaTime)
{
	CWidget::Update(DeltaTime);

	Brush.PlayAnimation(DeltaTime);
}

void CImage::Render()
{
	CWidget::Render();

	RenderBrush(Brush, RenderPos, Size);
}

bool CImage::CollideMouse(std::weak_ptr<CWidget>& Result, const FVector2& MousePos)
{
	return false;
}

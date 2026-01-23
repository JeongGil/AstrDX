#pragma once

#include "../EngineInfo.h"

struct FUIBrush
{
	std::weak_ptr<class CTexture> Texture;
	FColor Tint = FColor::White;
	std::vector<FTextureFrame> AnimationFrames;
	bool AnimationEnable = false;
	int	Frame = 0;
	float Time = 0.f;
	float FrameTime = 1.f;
	float PlayRate = 1.f;
	float PlayTime = 1.f;
};

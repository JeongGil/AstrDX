#include "CPostProcessBlur.h"

#include "../CDevice.h"
#include "../Asset/Shader/CCBufferBlur.h"

CPostProcessBlur::CPostProcessBlur()
{
}

CPostProcessBlur::~CPostProcessBlur()
{
}

bool CPostProcessBlur::Init()
{
	if (!CPostProcess::Init())
	{
		return false;
	}

	BlurCBuffer.reset(new CCBufferBlur);
	BlurCBuffer->Init();

	FVector2 TexelSize = CDevice::GetInst()->GetTexelSize();
	BlurCBuffer->SetTexelSize(TexelSize);
	SetBlurCount(5);

	CreateRenderTarget(DXGI_FORMAT_R8G8B8A8_UNORM, true);

	SetBlendTargetInfo(0);

	SetShader("Blur");

	return true;
}

void CPostProcessBlur::Update(const float DeltaTime)
{
}

void CPostProcessBlur::Render()
{
	BlurCBuffer->UpdateBuffer();
}

void CPostProcessBlur::ComputeGaussianWeight(int Count)
{
	BlurWeights.clear();
	BlurWeights.resize(Count + 1);

	float Sigma = Count * 0.5f;

	float Denom = 2.f * Sigma * Sigma;

	BlurWeights[0] = 1.f;

	float Sum = BlurWeights[0];

	for (int i = 1; i <= Count; ++i)
	{
		float Weight = std::exp(-(i * i) / Denom);
		BlurWeights[i] = Weight;
		Sum += 2.f * Weight;
	}

	for (auto& Weight : BlurWeights)
	{
		Weight /= Sum;
	}

	BlurCBuffer->SetWeight(BlurWeights);
}

void CPostProcessBlur::SetBlurCount(int Count)
{
	Count = min(Count, MAX_BLUR_COUNT);

	BlurCBuffer->SetCount(Count);

	ComputeGaussianWeight(Count);
}

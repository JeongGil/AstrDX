#pragma once
#include "CPostProcess.h"

class CCBufferBlur;
class CRenderManager;

class CPostProcessBlur :
    public CPostProcess
{
	friend CRenderManager;

protected:
	CPostProcessBlur();

public:
	~CPostProcessBlur() override;

private:
	std::shared_ptr<CCBufferBlur> BlurCBuffer;
	std::vector<float> BlurWeights;

public:
	bool Init() override;
	void Update(const float DeltaTime) override;

protected:
	void Render() override;

private:
	void ComputeGaussianWeight(int Count);

public:void SetBlurCount(int Count);
};


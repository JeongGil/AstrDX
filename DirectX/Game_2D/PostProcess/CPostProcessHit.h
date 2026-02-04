#pragma once
#include <Render/CPostProcess.h>

class CCBufferHit;
class CRenderManager;
class CRenderTarget;
class CTexture;

class CPostProcessHit :
    public CPostProcess
{
	friend CRenderManager;

protected:
	CPostProcessHit();

public:
	~CPostProcessHit() override;

private:
	float Elapsed{ 0.f };
	float Duration{ 0.1f };

	std::weak_ptr<CRenderTarget> MainTarget;
	std::weak_ptr<CTexture> HitTexture;
	FColor HitColor;
	std::shared_ptr<CCBufferHit> HitCBuffer;

public:
	bool Init() override;
	void Update(const float DeltaTime) override;

protected:
	void Render() override;

public:
	void SetEnableTime(float EnableTime)
	{
		this->Duration = EnableTime;
	}

	void SetHitColor(const FColor& Color);
	void SetHitColor(float r, float g, float b, float a);
};


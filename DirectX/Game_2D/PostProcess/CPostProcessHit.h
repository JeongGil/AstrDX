#pragma once
#include <Render/CPostProcess.h>

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
	float EnableTime{ 0.5f };

	std::weak_ptr<CRenderTarget> MainTarget;
	std::weak_ptr<CTexture> HitTexture;
	FColor Hitcolor;

public:
	bool Init() override;
	void Update(const float DeltaTime) override;

protected:
	void Render() override;

public:
	void SetEnableTime(float EnableTime)
	{
		this->EnableTime = EnableTime;
	}
};


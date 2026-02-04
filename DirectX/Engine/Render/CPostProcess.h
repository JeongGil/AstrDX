#pragma once

#include "../EngineInfo.h"
#include "../Asset/Shader/CBufferContainer.h"

class CRenderState;
class CShader;
class CRenderTarget;
class CRenderManager;

struct FTargetInfo
{
	std::weak_ptr<CRenderTarget> BlendTarget;
	int Register{ 0 };
	int ShaderBufferType{ EShaderBufferType::Pixel };
};

class CPostProcess
{
	friend CRenderManager;

protected:
	CPostProcess();

public:
	virtual ~CPostProcess() = 0;

protected:
	std::string Key;
	bool bEnable{ true };
	bool bActive{ true };
	int Order{ 0 };
	std::shared_ptr<CRenderTarget> Target;
	FTargetInfo BlendTarget;
	std::weak_ptr<CShader> Shader;
	std::shared_ptr<CRenderState> BlendState;

public:
	virtual bool Init();
	virtual void Update(const float DeltaTime) = 0;

protected:
	virtual void Render() = 0;
	void RenderFullScreenQuad();

private:
	void RenderPostProcess();

public:
	[[nodiscard]] std::weak_ptr<CRenderTarget> GetTarget() const
	{
		return Target;
	}

	[[nodiscard]] bool IsActive() const
	{
		return bActive;
	}

	void Destroy()
	{
		bActive = false;
	}

	[[nodiscard]] bool IsEnable() const
	{
		return bEnable;
	}

	void SetEnable(bool bEnable)
	{
		this->bEnable = bEnable;
	}

	[[nodiscard]] const std::string& GetKey() const
	{
		return Key;
	}

	void SetKey(const std::string& Key)
	{
		this->Key = Key;
	}

	[[nodiscard]] int GetOrder() const
	{
		return Order;
	}

	void SetOrder(int Order)
	{
		this->Order = Order;
	}

	void SetShader(const std::string& Key);
	void CreateRenderTarget(DXGI_FORMAT Format, bool bEnableDepth = false);
	void SetBlendTarget(const std::weak_ptr<CRenderTarget>& Target)
	{
		BlendTarget.BlendTarget = Target;
	}

	void SetBlendTargetInfo(int Register, int ShaderBufferType = EShaderBufferType::Pixel)
	{
		BlendTarget.Register = Register;
		BlendTarget.ShaderBufferType = ShaderBufferType;
	}

	void SetBlendState(const std::string& Key);
};


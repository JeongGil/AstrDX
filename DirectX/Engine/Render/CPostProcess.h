#pragma once

#include "../EngineInfo.h"

class CShader;
class CRenderTarget;
class CRenderManager;

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
	std::weak_ptr<CRenderTarget> Target;
	std::weak_ptr<CShader> Shader;

public:
	virtual bool Init();
	virtual void Update(const float DeltaTime) = 0;

protected:
	virtual void Render() = 0;
	void RenderFullScreenQuad();

private:
	void RenderPostProcess();

public:
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

	void SetRenderTarget(const std::weak_ptr<CRenderTarget>& Target);
	void SetRenderTarget(const std::string& Key);
	void SetShader(const std::string& Key);
};


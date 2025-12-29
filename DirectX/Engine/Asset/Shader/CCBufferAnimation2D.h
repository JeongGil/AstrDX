#pragma once
#include "CConstantBufferData.h"

class CCBufferAnimation2D :
    public CConstantBufferData
{
public:
	bool Init() override;
	void UpdateBuffer() override;
	CConstantBufferData* Clone() override;

	void SetLTUV(const FVector2& uv)
	{
		Data.LTUV = uv;
	}

	void SetLTUV(float u, float v)
	{
		Data.LTUV.x = u;
		Data.LTUV.y = v;
	}

	void SetRBUV(const FVector2& uv)
	{
		Data.RBUV = uv;
	}

	void SetRBUV(float u, float v)
	{
		Data.RBUV.x = u;
		Data.RBUV.y = v;
	}

	void SetEnableAnimation2D(bool bEnable)
	{
		Data.bEnableAnimation2D = bEnable ? 1 : 0;
	}

	void SetTextureType(EAnimation2DTextureType Type)
	{
		Data.TextureType = static_cast<int>(Type);
	}

	CCBufferAnimation2D() = default;
	CCBufferAnimation2D(const CCBufferAnimation2D& other);
	CCBufferAnimation2D(CCBufferAnimation2D&& other) noexcept;
	CCBufferAnimation2D& operator=(const CCBufferAnimation2D& other);
	CCBufferAnimation2D& operator=(CCBufferAnimation2D&& other) noexcept;
	~CCBufferAnimation2D() override = default;

private:
	FCBufferAnimation2DData Data;
};


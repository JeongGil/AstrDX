#pragma once
#include "CConstantBufferData.h"

class CCBufferUIDefault :
    public CConstantBufferData
{
public:
	CCBufferUIDefault();
	CCBufferUIDefault(const CCBufferUIDefault& Other) = default;
	CCBufferUIDefault(CCBufferUIDefault&& Other) noexcept = default;
	CCBufferUIDefault& operator=(const CCBufferUIDefault& Other) = default;
	CCBufferUIDefault& operator=(CCBufferUIDefault&& Other) noexcept = default;
	~CCBufferUIDefault() override;

private:
	FCBufferUIDefaultData Data;

public:
	bool Init() override;
	void UpdateBuffer() override;
	CCBufferUIDefault* Clone() override;

	void SetBrushTint(const FColor& Color)
	{
		Data.BrushTint = Color;
	}

	void SetWidgetColor(const FColor& Color)
	{
		Data.WidgetColor = Color;
	}

	void SetBrushLTUV(const FVector2& UV)
	{
		Data.BrushLTUV = UV;
	}

	void SetBrushRBUV(const FVector2& UV)
	{
		Data.BrushRBUV = UV;
	}

	void SetAnimEnable(bool bEnable)
	{
		Data.BrushAnimEnable = bEnable ? 1 : 0;
	}

	void SetTextureEnable(bool bEnable)
	{
		Data.BrushTextureEnable = bEnable ? 1 : 0;
	}
};


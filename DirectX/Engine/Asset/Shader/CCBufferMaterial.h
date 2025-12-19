#pragma once
#include "CConstantBufferData.h"

class CCBufferMaterial :
    public CConstantBufferData
{
public:
	void SetBaseColor(const FVector4& Color)
	{
		Data.BaseColor = Color;
	}

	void SetOpacity(float Opacity)
	{
		Data.Opacity = Opacity;
	}

private:
	FCBufferMaterialData Data;

public:
	bool Init() override;
	void UpdateBuffer() override;
	CCBufferMaterial* Clone() override;

public:
	CCBufferMaterial() = default;

	CCBufferMaterial(const CCBufferMaterial& other)
		: CConstantBufferData(other),
		  Data(other.Data)
	{
	}

	CCBufferMaterial(CCBufferMaterial&& other) noexcept
		: CConstantBufferData(std::move(other)),
		  Data(std::move(other.Data))
	{
	}

	CCBufferMaterial& operator=(const CCBufferMaterial& other)
	{
		if (this == &other)
			return *this;
		CConstantBufferData::operator =(other);
		Data = other.Data;
		return *this;
	}

	CCBufferMaterial& operator=(CCBufferMaterial&& other) noexcept
	{
		if (this == &other)
			return *this;
		CConstantBufferData::operator =(std::move(other));
		Data = std::move(other.Data);
		return *this;
	}

	~CCBufferMaterial() override = default;
};


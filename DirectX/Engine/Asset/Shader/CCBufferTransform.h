#pragma once

#include "CConstantBufferData.h"

class CCBufferTransform :
	public CConstantBufferData
{
public:
	bool Init() override;
	void UpdateBuffer() override;
	CCBufferTransform* Clone() override;

public:
	void SetWorldMatrix(const FMatrix& World)
	{
		Data.World = World;
	}

	void SetViewMatrix(const FMatrix& View)
	{
		Data.View = View;
	}

	void SetProjMatrix(const FMatrix& Proj)
	{
		Data.Proj = Proj;
	}

private:
	FCBufferTransformData Data;

public:
	CCBufferTransform() = default;
	~CCBufferTransform() override = default;

	CCBufferTransform(const CCBufferTransform& other)
		: CConstantBufferData(other),
		  Data(other.Data)
	{
	}

	CCBufferTransform(CCBufferTransform&& other) noexcept
		: CConstantBufferData(std::move(other)),
		  Data(std::move(other.Data))
	{
	}

	CCBufferTransform& operator=(const CCBufferTransform& other)
	{
		if (this == &other)
			return *this;
		CConstantBufferData::operator =(other);
		Data = other.Data;
		return *this;
	}

	CCBufferTransform& operator=(CCBufferTransform&& other) noexcept
	{
		if (this == &other)
			return *this;
		CConstantBufferData::operator =(std::move(other));
		Data = std::move(other.Data);
		return *this;
	}
};


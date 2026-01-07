#pragma once
#include "CConstantBufferData.h"
class CCBufferCollider :
    public CConstantBufferData
{
public:
	void SetColor(const FColor& Color)
	{
		Data.Color = Color;
	}

private:
	FCBufferColliderData Data;

public:
	bool Init() override;
	void UpdateBuffer() override;
	CCBufferCollider* Clone() override;

public:
	CCBufferCollider() = default;

	CCBufferCollider(const CCBufferCollider& other)
		: CConstantBufferData(other),
		  Data(other.Data)
	{
	}

	CCBufferCollider(CCBufferCollider&& other) noexcept
		: CConstantBufferData(std::move(other)),
		  Data(std::move(other.Data))
	{
	}

	CCBufferCollider& operator=(const CCBufferCollider& other)
	{
		if (this == &other)
			return *this;
		CConstantBufferData::operator =(other);
		Data = other.Data;
		return *this;
	}

	CCBufferCollider& operator=(CCBufferCollider&& other) noexcept
	{
		if (this == &other)
			return *this;
		CConstantBufferData::operator =(std::move(other));
		Data = std::move(other.Data);
		return *this;
	}

public:
	~CCBufferCollider() override = default;
};


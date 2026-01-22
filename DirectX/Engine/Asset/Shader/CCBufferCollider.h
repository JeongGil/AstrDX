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
	CCBufferCollider(const CCBufferCollider& other) = default;
	CCBufferCollider(CCBufferCollider&& other) noexcept = default;
	CCBufferCollider& operator=(const CCBufferCollider& other) = default;
	CCBufferCollider& operator=(CCBufferCollider&& other) noexcept = default;

public:
	~CCBufferCollider() override = default;
};


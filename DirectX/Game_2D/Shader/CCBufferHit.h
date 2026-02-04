#pragma once
#include <Asset/Shader/CConstantBufferData.h>

struct FCBufferHitData
{
	FColor Color;
};

class CCBufferHit :
    public CConstantBufferData
{
public:
	CCBufferHit();
	CCBufferHit(const CCBufferHit& Other);
	CCBufferHit(CCBufferHit&& Other) noexcept;
	~CCBufferHit() override;

private:
	FCBufferHitData Data;

public:
	bool Init() override;
	void UpdateBuffer() override;
	CCBufferHit* Clone() override;

	void SetColor(const FColor& Color)
	{
		Data.Color = Color;
	}
};


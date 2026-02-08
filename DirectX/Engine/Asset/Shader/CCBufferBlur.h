#pragma once
#include "CConstantBufferData.h"

class CCBufferBlur :
	public CConstantBufferData
{
public:
	CCBufferBlur();
	CCBufferBlur(const CCBufferBlur& other);
	CCBufferBlur(CCBufferBlur&& other) noexcept;
	~CCBufferBlur() override;

private:
	FCBufferBlur Data;

public:
	bool Init() override;
	void UpdateBuffer() override;
	CCBufferBlur* Clone() override;

	void SetTexelSize(const FVector2& TexelSize)
	{
		Data.TexelSize = TexelSize;
	}

	void SetCount(int Count)
	{
		Data.Count = Count;
	}

	void SetWeight(const std::vector<float>& Weights)
	{
		if (Weights.size() > MAX_BLUR_COUNT + 1)
			return;

		memcpy(Data.BlurWeight, &Weights[0], sizeof(float) * Weights.size());
	}
};


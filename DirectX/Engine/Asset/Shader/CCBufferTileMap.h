#pragma once
#include "CConstantBufferData.h"

class CCBufferTileMap :
    public CConstantBufferData
{
public:
	CCBufferTileMap();
	CCBufferTileMap(const CCBufferTileMap& Other);
	CCBufferTileMap(CCBufferTileMap&& Other) noexcept;
	~CCBufferTileMap() override;

private:
	FCBufferTileMapData Data;

public:
	bool Init() override;
	void UpdateBuffer() override;
	CCBufferTileMap* Clone() override;

	void SetUV(const FVector2& LT, const FVector2& RB)
	{
		Data.LTUV = LT;
		Data.RBUV = RB;
	}
};


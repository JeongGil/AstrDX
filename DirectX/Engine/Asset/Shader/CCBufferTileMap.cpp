#include "CCBufferTileMap.h"

#include "CConstantBuffer.h"

CCBufferTileMap::CCBufferTileMap()
{
}

CCBufferTileMap::CCBufferTileMap(const CCBufferTileMap& Other) :
	CConstantBufferData(Other),
	Data(Other.Data)
{
}

CCBufferTileMap::CCBufferTileMap(CCBufferTileMap&& Other) noexcept :
	CConstantBufferData(std::move(Other)),
	Data(std::move(Other.Data))
{
}

CCBufferTileMap::~CCBufferTileMap()
{
}

bool CCBufferTileMap::Init()
{
	SetConstantBuffer("TileMap");

	return true;
}

void CCBufferTileMap::UpdateBuffer()
{
	if (auto CBuffer = Buffer.lock())
	{
		CBuffer->Update(&Data);
	}
}

CCBufferTileMap* CCBufferTileMap::Clone()
{
	return new CCBufferTileMap(*this);
}

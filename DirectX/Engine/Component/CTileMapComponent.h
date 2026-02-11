#pragma once
#include "CObjectComponent.h"

class CTile;
class CCBufferTileMap;
class CShader;
class CCBufferTransform;
class CMesh;
class CGameObject;

struct FTileMapInstancingBuffer
{
	FVector4 WVP0;
	FVector4 WVP1;
	FVector4 WVP2;
	FVector4 WVP3;
	FVector2 LTUV;
	FVector2 RBUV;
};

class CTileMapComponent :
	public CObjectComponent
{
	friend CGameObject;

protected:
	CTileMapComponent();
	CTileMapComponent(const CTileMapComponent& other);
	CTileMapComponent(CTileMapComponent&& other) noexcept;

public:
	~CTileMapComponent() override;

protected:
	std::vector<std::shared_ptr<CTile>>	Tiles;
	std::vector<FTileMapInstancingBuffer> TileInstData;
	FVertexBuffer InstancingBuffer;
	int InstancingCount{ 0 };
	ETileShape Shape{ Rect };
	FVector2 TileSize;
	FVector2 MapSize;
	int CountX{ 0 };
	int CountY{ 0 };
	bool bRenderTileOutLine{ false };
	std::weak_ptr<CMesh> OutLineMesh;
	std::weak_ptr<CShader> OutLineShader;
	std::weak_ptr<CMesh> TileMesh;
	std::weak_ptr<CShader> TileShader;
	std::shared_ptr<CCBufferTransform> Transform;
	std::shared_ptr<CCBufferTileMap> CBufferTileMap;
	FVector2 TileTextureSize;
	std::vector<FTileFrame> TileFrames;

	int ViewStartX{ 0 };
	int ViewStartY{ 0 };
	int ViewEndX{ 0 };
	int ViewEndY{ 0 };

public:
	bool Init() override;
	virtual bool Init(const char* FileName);
	void Update(const float DeltaTime) override;
	void PostUpdate(const float DeltaTime) override;
	void PostRender() override;
	void Destroy() override;

protected:
	CTileMapComponent* Clone() const override;

public:
	void SetTileOutLineRender(bool bRender);
	bool SetTileTexture(ETileTextureType::Type Type, const std::weak_ptr<class CTexture>& Texture);
	bool SetTileTexture(ETileTextureType::Type Type, const std::string& Key);
	bool SetTileTexture(ETileTextureType::Type Type, const std::string& Key, const TCHAR* FileName, const std::string& PathName = "Texture");
	void AddTileFrame(const FVector2& Start, const FVector2& End);
	void AddTileFrame(float StartX, float StartY, float EndX, float EndY);
	void SetTileFrameAll(int FrameIndex);

	void RenderTile();
	void RenderTileOutLine();

	void CreateTile(ETileShape Shape, int CountX, int CountY, const FVector2& TileSize, int TileTextureFrame = -1);

private:
	bool CreateInstancingBuffer(int Size, int Count);
	bool SetInstancingData(void* Data, int Count);

public:
	[[nodiscard]] ETileShape GetTileShape() const
	{
		return Shape;
	}

	[[nodiscard]] FVector2 GetTileSize() const
	{
		return TileSize;
	}

	[[nodiscard]] FVector2 GetMapSize() const
	{
		return MapSize;
	}

	[[nodiscard]] int GetTileCountX() const
	{
		return CountX;
	}

	[[nodiscard]] int GetTileCountY() const
	{
		return CountY;
	}

	void SetTileTextureSize(const FVector2& Size)
	{
		TileTextureSize = Size;
	}

	void SetTileTextureSize(float x, float y)
	{
		SetTileTextureSize(FVector2(x, y));
	}
};


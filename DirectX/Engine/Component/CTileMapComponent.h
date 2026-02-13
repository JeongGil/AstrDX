#pragma once
#include "CObjectComponent.h"

class CTileMapRender;
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
	FColor Color{ FColor::White };
};

class CTileMapComponent :
	public CObjectComponent
{
	friend CGameObject;

protected:
	CTileMapComponent();
	CTileMapComponent(const CTileMapComponent& other) = default;
	CTileMapComponent(CTileMapComponent&& other) noexcept = default;

public:
	~CTileMapComponent() override;

protected:
	std::vector<std::shared_ptr<CTile>>	Tiles;
	std::vector<FTileMapInstancingBuffer> TileInstData;
	std::vector<FTileMapInstancingBuffer> TileLineInstData;
	FVertexBuffer InstancingBuffer;
	FVertexBuffer LineInstancingBuffer;
	int InstancingCount{ 0 };
	int LineInstancingCount{ 0 };
	ETileShape Shape{ Rect };
	FVector2 TileSize;
	FVector2 MapSize;
	int CountX{ 0 };
	int CountY{ 0 };
	bool bRenderTileOutline{ false };
	std::weak_ptr<CMesh> OutlineMesh;
	std::weak_ptr<CShader> OutlineShader;
	std::weak_ptr<CMesh> TileMesh;
	std::weak_ptr<CShader> TileShader;
	std::shared_ptr<CCBufferTransform> Transform;
	std::shared_ptr<CCBufferTileMap> CBufferTileMap;
	FVector2 TileTextureSize;
	std::vector<FTileFrame> TileFrames;

	std::weak_ptr<CTileMapRender> TileMapRender;

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
	void SetTileOutlineRender(bool bRender);
	void SetTileOutlineRender(bool bRender, int Index);
	void SetOutlineMesh(const std::string& Key);
	void SetOutlineShader(const std::string& Key);
	void SetTileMesh(const std::string& Key);
	void SetTileShader(const std::string& Key);
	bool SetTileTexture(ETileTextureType::Type Type, const std::weak_ptr<class CTexture>& Texture);
	bool SetTileTexture(ETileTextureType::Type Type, const std::string& Key);
	bool SetTileTexture(ETileTextureType::Type Type, const std::string& Key, const TCHAR* FileName, const std::string& PathName = "Texture");
	void AddTileFrame(const FVector2& Start, const FVector2& End);
	void AddTileFrame(float StartX, float StartY, float EndX, float EndY);
	void SetTileFrameAll(int FrameIndex);

	void RenderTile();
	void RenderTileOutLine();

	void CreateTile(ETileShape Shape, int CountX, int CountY, const FVector2& TileSize,
		int TileTextureFrame = -1, bool bOutLineRender = false);

	void Save(const TCHAR* FileName, const std::string& PathName = "Asset");
	void SaveFullPath(const TCHAR* FullPath);
	void Load(const TCHAR* FileName, const std::string& PathName = "Asset");
	void LoadFullPath(const TCHAR* FullPath);

private:
	bool CreateInstancingBuffer(int Size, int Count);
	bool SetInstancingData(void* Data, int Count);

	bool CreateLineInstancingBuffer(int Size, int Count);
	bool SetLineInstancingData(void* Data, int Count);

	int GetTileRenderIndexX(const FVector2& Pos) const;
	int GetTileRenderIndexY(const FVector2& Pos) const;

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

	[[nodiscard]] size_t GetTileFrameCount() const
	{
		return TileFrames.size();
	}

	int GetTileIndex(const FVector2& Pos) const;
	int GetTileIndex(const FVector& Pos) const;
	int GetTileIndex(float x, float y) const;
	std::weak_ptr<CTile> GetTile(const FVector2& Pos) const;
	std::weak_ptr<CTile> GetTile(const FVector& Pos) const;
	std::weak_ptr<CTile> GetTile(float x, float y) const;
	std::weak_ptr<CTile> GetTile(int Index) const
	{
		if (Index < 0 || Index >= static_cast<int>(Tiles.size()))
		{
			return {};
		}

		return Tiles[Index];
	}

	void SetTileFrame(int Index, int Frame);

	void SetTileTextureSize(const FVector2& Size)
	{
		TileTextureSize = Size;
	}

	void SetTileTextureSize(float x, float y)
	{
		SetTileTextureSize(FVector2(x, y));
	}

	void SetTileMapRender(const std::weak_ptr<CTileMapRender>& Render)
	{
		TileMapRender = Render;
	}
};


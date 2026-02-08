#include "CTileMapComponent.h"

CTileMapComponent::CTileMapComponent()
{
}

CTileMapComponent::CTileMapComponent(const CTileMapComponent& other) :
	CObjectComponent(other),
	Tiles(other.Tiles),
	Shape(other.Shape),
	TileSize(other.TileSize),
	MapSize(other.MapSize),
	CountX(other.CountX),
	CountY(other.CountY),
	TileOutLineRender(other.TileOutLineRender),
	OutLineMesh(other.OutLineMesh),
	OutLineShader(other.OutLineShader),
	TileMesh(other.TileMesh),
	TileShader(other.TileShader),
	Transform(other.Transform),
	CBufferTileMap(other.CBufferTileMap),
	TileTextureSize(other.TileTextureSize),
	TileFrames(other.TileFrames),
	ViewStartX(other.ViewStartX),
	ViewStartY(other.ViewStartY),
	ViewEndX(other.ViewEndX),
	ViewEndY(other.ViewEndY)
{
}

CTileMapComponent::CTileMapComponent(CTileMapComponent&& other) noexcept :
	CObjectComponent(std::move(other)),
	Tiles(std::move(other.Tiles)),
	Shape(other.Shape),
	TileSize(std::move(other.TileSize)),
	MapSize(std::move(other.MapSize)),
	CountX(other.CountX),
	CountY(other.CountY),
	TileOutLineRender(other.TileOutLineRender),
	OutLineMesh(std::move(other.OutLineMesh)),
	OutLineShader(std::move(other.OutLineShader)),
	TileMesh(std::move(other.TileMesh)),
	TileShader(std::move(other.TileShader)),
	Transform(std::move(other.Transform)),
	CBufferTileMap(std::move(other.CBufferTileMap)),
	TileTextureSize(std::move(other.TileTextureSize)),
	TileFrames(std::move(other.TileFrames)),
	ViewStartX(other.ViewStartX),
	ViewStartY(other.ViewStartY),
	ViewEndX(other.ViewEndX),
	ViewEndY(other.ViewEndY)
{
}

CTileMapComponent::~CTileMapComponent()
{
}

bool CTileMapComponent::Init()
{
	return CObjectComponent::Init();
}

bool CTileMapComponent::Init(const char* FileName)
{
}

void CTileMapComponent::Update(const float DeltaTime)
{
	CObjectComponent::Update(DeltaTime);
}

void CTileMapComponent::PostUpdate(const float DeltaTime)
{
	CObjectComponent::PostUpdate(DeltaTime);
}

void CTileMapComponent::PostRender()
{
	CObjectComponent::PostRender();
}

void CTileMapComponent::Destroy()
{
	CObjectComponent::Destroy();
}

CTileMapComponent* CTileMapComponent::Clone() const
{
}

void CTileMapComponent::SetTileOutLineRender(bool bRender)
{
}

bool CTileMapComponent::SetTileTexture(ETileTextureType::Type Type, const std::weak_ptr<class CTexture>& Texture)
{
}

bool CTileMapComponent::SetTileTexture(ETileTextureType::Type Type, const std::string& Key)
{
}

bool CTileMapComponent::SetTileTexture(ETileTextureType::Type Type, const std::string& Key, const TCHAR* FileName,
	const std::string& PathName)
{
}

void CTileMapComponent::AddTileFrame(const FVector2& Start, const FVector2& End)
{
}

void CTileMapComponent::AddTileFrame(float StartX, float StartY, float EndX, float EndY)
{
}

void CTileMapComponent::SetTileFrameAll(int FrameIndex)
{
}

void CTileMapComponent::RenderTile()
{
}

void CTileMapComponent::RenderTileOutLine()
{
}

void CTileMapComponent::CreateTile(ETileShape Shape, int CountX, int CountY, const FVector2& TileSize,
	int TileTextureFrame)
{
}

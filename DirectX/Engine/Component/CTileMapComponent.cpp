#include "CTileMapComponent.h"

#include "CTile.h"
#include "CTileMapRender.h"
#include "../CDevice.h"
#include "../Asset/CAssetManager.h"
#include "../Asset/Mesh/CMeshManager.h"
#include "../Asset/Shader/CCBufferTileMap.h"
#include "../Asset/Shader/CCBufferTransform.h"
#include "../Asset/Shader/CShaderManager.h"
#include "../Object/CGameObject.h"
#include "../World/CWorld.h"
#include "../World/CWorldManager.h"

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
	bRenderTileOutLine(other.bRenderTileOutLine),
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
	bRenderTileOutLine(other.bRenderTileOutLine),
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
	if (!CObjectComponent::Init())
	{
		return false;
	}

	if (auto Owner = this->Owner.lock())
	{
		if (auto Renderer = Owner->GetComponent<CTileMapRender>().lock())
		{
			Renderer->SetTileMapComponent(std::dynamic_pointer_cast<CTileMapComponent>(shared_from_this()));
		}
	}

	Transform.reset(new CCBufferTransform);
	Transform->Init();

	CBufferTileMap.reset(new CCBufferTileMap);
	CBufferTileMap->Init();

	if (auto World = this->World.lock())
	{
		if (auto AssetMgr = World->GetWorldAssetManager().lock())
		{
			TileMesh = AssetMgr->FindMesh("RectTex");
		}
	}
	else
	{
		if (auto MeshMgr = CAssetManager::GetInst()->GetMeshManager().lock())
		{
			TileMesh = MeshMgr->FindMesh("Mesh_RectTex");
		}
	}

	if (auto ShaderMgr = CAssetManager::GetInst()->GetShaderManager().lock())
	{
		TileShader = ShaderMgr->FindShader("TileMap");
	}

	return true;
}

bool CTileMapComponent::Init(const char* FileName)
{
	return true;
}

void CTileMapComponent::Update(const float DeltaTime)
{
}

void CTileMapComponent::PostUpdate(const float DeltaTime)
{
	auto World = this->World.lock();

	if (!World)
	{
		World = CWorldManager::GetInst()->GetWorld().lock();
	}

	FVector Center = World->GetCameraManager().lock()->GetMainCameraWorldPosition();
	Center -= Owner.lock()->GetWorldPosition();

	FResolution RS = CDevice::GetInst()->GetResolution();

	switch (Shape)
	{
		case Rect:
			ViewStartX = (int)((Center.x - RS.Width * 0.5f) / TileSize.x);
			ViewStartY = (int)((Center.y - RS.Height * 0.5f) / TileSize.y);

			ViewEndX = (int)((Center.x + RS.Width * 0.5f) / TileSize.x);
			ViewEndY = (int)((Center.y + RS.Height * 0.5f) / TileSize.y);
			break;
		case Isometric:
			break;
	}

	ViewStartX = std::clamp(ViewStartX, 0, CountX - 1);
	ViewEndX = std::clamp(ViewEndX, 0, CountX - 1);

	ViewStartY = std::clamp(ViewStartY, 0, CountY - 1);
	ViewEndY = std::clamp(ViewEndY, 0, CountY - 1);

	int	DataCountX = ViewEndX - ViewStartX + 1;
	int	DataCountY = ViewEndY - ViewStartY + 1;

	InstancingCount = DataCountX * DataCountY;

	if (TileInstData.size() < InstancingCount)
	{
		TileInstData.clear();
		TileInstData.resize(InstancingCount);
	}

	auto Owner = this->Owner.lock();
	auto CameraMgr = World->GetCameraManager().lock();

	InstancingCount = 0;

	for (int i = ViewStartY; i <= ViewEndY; ++i)
	{
		for (int j = ViewStartX; j <= ViewEndX; ++j)
		{
			int	Idx = i * CountX + j;
			auto& Tile = Tiles[Idx];

			if (!Tile->IsRender())
			{
				continue;
			}

			FMatrix	ScaleMat, TranslateMat;

			ScaleMat.Scaling(TileSize);

			FVector2 Pos = Tile->GetPos();

			Pos.x += Owner->GetWorldPosition().x;
			Pos.y += Owner->GetWorldPosition().y;

			TranslateMat.Translation(Pos);

			FMatrix	WorldMat = ScaleMat * TranslateMat;

			//mTileIstData[mInstancingCount].WVP0

			/*mTransform->SetWorldMatrix(WorldMat);

			mTransform->SetViewMatrix(CameraMgr->GetViewMatrix());
			mTransform->SetProjMatrix(CameraMgr->GetProjMatrix());*/
		}
	}
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
	return new CTileMapComponent(*this);
}

void CTileMapComponent::SetTileOutLineRender(bool bRender)
{
	bRenderTileOutLine = bRender;

	if (bRenderTileOutLine)
	{

	}
	else
	{
		OutLineMesh.reset();
		OutLineShader.reset();
	}
}

bool CTileMapComponent::SetTileTexture(ETileTextureType::Type Type, const std::weak_ptr<class CTexture>& Texture)
{
	if (auto Owner = this->Owner.lock())
	{
		if (auto Renderer = Owner->GetComponent<CTileMapRender>().lock())
		{
			return Renderer->SetTexture(Type, Texture);
		}
	}

	return false;
}

bool CTileMapComponent::SetTileTexture(ETileTextureType::Type Type, const std::string& Key)
{
	if (auto Owner = this->Owner.lock())
	{
		if (auto Renderer = Owner->GetComponent<CTileMapRender>().lock())
		{
			return Renderer->SetTexture(Type, Key);
		}
	}

	return false;
}

bool CTileMapComponent::SetTileTexture(ETileTextureType::Type Type, const std::string& Key, const TCHAR* FileName,
	const std::string& PathName)
{
	if (auto Owner = this->Owner.lock())
	{
		if (auto Renderer = Owner->GetComponent<CTileMapRender>().lock())
		{
			return Renderer->SetTexture(Type, Key, FileName, PathName);
		}
	}

	return false;
}

void CTileMapComponent::AddTileFrame(const FVector2& Start, const FVector2& End)
{
	TileFrames.emplace_back(Start, End);
}

void CTileMapComponent::AddTileFrame(float StartX, float StartY, float EndX, float EndY)
{
	AddTileFrame(FVector2(StartX, StartY), FVector2(EndX, EndY));
}

void CTileMapComponent::SetTileFrameAll(int FrameIndex)
{
	const auto Frame = TileFrames[FrameIndex];
	for (const auto& Tile : Tiles)
	{
		Tile->SetFrame(Frame.Start, Frame.End);
	}
}

void CTileMapComponent::RenderTile()
{
	auto Owner = this->Owner.lock();
	auto Shader = TileShader.lock();
	auto Mesh = TileMesh.lock();
	if (!Owner || !Shader || !Mesh)
	{
		return;
	}

	auto World = this->World.lock();
	if (!World)
	{
		World = CWorldManager::GetInst()->GetWorld().lock();
	}

	auto CameraMgr = World->GetCameraManager().lock();
	if (!CameraMgr)
	{
		return;
	}

	for (int i = ViewStartY; i <= ViewEndY; i++)
	{
		for (int j = ViewStartX; j <= ViewEndX; j++)
		{
			int Idx = i * CountX + j;

			const auto& Tile = Tiles[Idx];
			if (!Tile->IsRender())
			{
				continue;
			}

			FMatrix	ScaleMat, TranslateMat, WorldMat;

			ScaleMat.Scaling(TileSize);

			FVector2	Pos = Tile->GetPos();

			Pos.x += Owner->GetWorldPosition().x;
			Pos.y += Owner->GetWorldPosition().y;

			TranslateMat.Translation(Pos);

			WorldMat = ScaleMat * TranslateMat;

			Transform->SetWorldMatrix(WorldMat);

			Transform->SetViewMatrix(CameraMgr->GetViewMatrix());
			Transform->SetProjMatrix(CameraMgr->GetProjMatrix());

			Transform->UpdateBuffer();

			FVector2 LTUV = Tile->GetFrameStart() / TileTextureSize;
			FVector2 RBUV = Tile->GetFrameEnd() / TileTextureSize;

			CBufferTileMap->SetUV(LTUV, RBUV);

			CBufferTileMap->UpdateBuffer();

			Shader->SetShader();

			Mesh->Render();
		}
	}

	// Determines the maximum number of tiles that can be visible on the screen.
	FResolution	RS = CDevice::GetInst()->GetResolution();

	int	ViewCountX = RS.Width / TileSize.x + 3;
	int	ViewCountY = RS.Height / TileSize.y + 3;

	if (Mesh)
	{
		Mesh->CreateInstancingBuffer(sizeof(FTileMapInstancingBuffer), ViewCountX * ViewCountY);

		TileInstData.resize(ViewCountX * ViewCountY);
	}
}

void CTileMapComponent::RenderTileOutLine()
{
}

void CTileMapComponent::CreateTile(ETileShape Shape, int CountX, int CountY, const FVector2& TileSize,
	int TileTextureFrame)
{
	this->Shape = Shape;
	this->CountX = CountX;
	this->CountY = CountY;
	this->TileSize = TileSize;

	switch (Shape)
	{
		case Rect:
			MapSize = TileSize * FVector2((float)CountX, (float)CountY);
			break;
		case Isometric:
			break;
	}

	auto Owner = this->Owner.lock();

	if (auto Renderer = Owner->GetComponent<CTileMapRender>().lock())
	{
		Renderer->SetWorldScale(MapSize);
	}

	Tiles.clear();

	Tiles.resize(CountX * CountY);

	for (int i = 0; i < CountY; ++i)
	{
		for (int j = 0; j < CountX; ++j)
		{
			int	Index = i * CountX + j;

			auto& Tile = Tiles[Index];
			Tile.reset<CTile>(new CTile);

			switch (Shape)
			{
				case Rect:
					Tile->SetPos(j * TileSize.x, i * TileSize.y);
					break;
				case Isometric:
					break;
			}

			Tile->SetSize(TileSize);
			Tile->SetCenter(Tile->GetPos() + TileSize * 0.5f);
			Tile->SetTextureFrame(TileTextureFrame);
		}
	}
}

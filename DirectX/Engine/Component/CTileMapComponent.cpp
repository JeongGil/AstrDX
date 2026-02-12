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
			OutLineMesh = AssetMgr->FindMesh("LBFrameRect");
		}
	}
	else
	{
		if (auto MeshMgr = CAssetManager::GetInst()->GetMeshManager().lock())
		{
			TileMesh = MeshMgr->FindMesh("Mesh_RectTex");
			OutLineMesh = MeshMgr->FindMesh("Mesh_LBFrameRect");
		}
	}

	if (auto ShaderMgr = CAssetManager::GetInst()->GetShaderManager().lock())
	{
		//TileShader = ShaderMgr->FindShader("TileMap");
		TileShader = ShaderMgr->FindShader("TileMapInstancing");
		OutLineShader = ShaderMgr->FindShader("TileMapLineInstancing");
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
	LineInstancingCount = 0;

	for (int i = ViewStartY; i <= ViewEndY; ++i)
	{
		for (int j = ViewStartX; j <= ViewEndX; ++j)
		{
			int	Idx = i * CountX + j;
			auto& Tile = Tiles[Idx];

			if (Tile->GetRender())
			{
				FMatrix	ScaleMat, TranslateMat;

				ScaleMat.Scaling(TileSize);

				FVector2 Pos = Tile->GetPos();

				Pos.x += Owner->GetWorldPosition().x;
				Pos.y += Owner->GetWorldPosition().y;

				TranslateMat.Translation(Pos);

				//FMatrix	RotMat;

				//RotMat.RotationZ(230.f);
				//RotMat.Rotation(FVector3(0.f, 0.f, 238.f));

				//WorldMat = ScaleMat * RotMat * TranslateMat;
				FMatrix	WorldMat = ScaleMat * TranslateMat;

				FMatrix	ViewMat = CameraMgr->GetViewMatrix();
				FMatrix	ProjMat = CameraMgr->GetProjMatrix();

				FMatrix	WVPMat = WorldMat * ViewMat * ProjMat;

				auto& InstancingData = TileInstData[InstancingCount];
				InstancingData.WVP0 = WVPMat[0];
				InstancingData.WVP1 = WVPMat[1];
				InstancingData.WVP2 = WVPMat[2];
				InstancingData.WVP3 = WVPMat[3];

				InstancingData.LTUV = Tile->GetFrameStart() / TileTextureSize;
				InstancingData.RBUV = Tile->GetFrameEnd() / TileTextureSize;

				++InstancingCount;
			}

			if (Tile->GetOutlineRender())
			{
				auto& LineInstancingData = TileLineInstData[LineInstancingCount];
				const auto& TileInstancingData = TileInstData[InstancingCount - 1];

				LineInstancingData.WVP0 = TileInstancingData.WVP0;
				LineInstancingData.WVP1 = TileInstancingData.WVP1;
				LineInstancingData.WVP2 = TileInstancingData.WVP2;
				LineInstancingData.WVP3 = TileInstancingData.WVP3;

				LineInstancingData.Color = Tiles[Idx]->GetOutlineColor();

				++LineInstancingCount;
			}
		}
	}

	SetInstancingData(&TileInstData[0], InstancingCount);
	SetLineInstancingData(&TileLineInstData[0], LineInstancingCount);
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

void CTileMapComponent::SetTileOutlineRender(bool bRender)
{
	for (const auto& Tile : Tiles)
	{
		Tile->SetOutlineRender(bRender);
	}
	//bRenderTileOutLine = bRender;

	//if (bRenderTileOutLine)
	//{

	//}
	//else
	//{
	//	OutLineMesh.reset();
	//	OutLineShader.reset();
	//}
}

void CTileMapComponent::SetTileOutlineRender(bool bRender, int Index)
{
	Tiles[Index]->SetOutlineRender(bRender);
}

bool CTileMapComponent::CreateLineInstancingBuffer(int Size, int Count)
{
	SAFE_RELEASE(LineInstancingBuffer.Buffer);

	LineInstancingBuffer.Size = Size;
	LineInstancingBuffer.Count = Count;

	D3D11_BUFFER_DESC BufferDesc{};
	BufferDesc.ByteWidth = Size * Count;
	BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;


	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&BufferDesc, nullptr, &LineInstancingBuffer.Buffer)))
	{
		return false;
	}

	return true;
}

bool CTileMapComponent::SetLineInstancingData(void* Data, int Count)
{
	if (!LineInstancingBuffer.Buffer)
	{
		return false;
	}

	if (LineInstancingBuffer.Count < Count)
	{
		if (!CreateLineInstancingBuffer(LineInstancingBuffer.Size, Count * 2))
		{
			return false;
		}
	}

	ID3D11DeviceContext* Context = CDevice::GetInst()->GetContext();

	D3D11_MAPPED_SUBRESOURCE MS{};

	Context->Map(LineInstancingBuffer.Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MS);

	memcpy(MS.pData, Data, LineInstancingBuffer.Size * Count);

	Context->Unmap(LineInstancingBuffer.Buffer, 0);

	LineInstancingCount = Count;

	return true;
}

int CTileMapComponent::GetTileIndex(const FVector2& Pos) const
{
	return GetTileIndex(Pos.x, Pos.y);
}

int CTileMapComponent::GetTileIndex(const FVector& Pos) const
{
	return GetTileIndex(Pos.x, Pos.y);
}

int CTileMapComponent::GetTileIndex(float x, float y) const
{
	auto Owner = this->Owner.lock();
	if (!Owner)
	{
		return -1;
	}

	FVector OwnerPos = Owner->GetWorldPosition();

	x = x - OwnerPos.x;
	y = y - OwnerPos.y;

	int	IndexX = x / TileSize.x;
	int	IndexY = y / TileSize.y;

	return IndexY * CountX + IndexX;
}

std::weak_ptr<CTile> CTileMapComponent::GetTile(const FVector2& Pos) const
{
	return GetTile(GetTileIndex(Pos));
}

std::weak_ptr<CTile> CTileMapComponent::GetTile(const FVector& Pos) const
{
	return GetTile(GetTileIndex(Pos));
}

std::weak_ptr<CTile> CTileMapComponent::GetTile(float x, float y) const
{
	return GetTile(GetTileIndex(x, y));
}

void CTileMapComponent::SetTileFrame(int Index, int Frame)
{
	auto& TileFrame = TileFrames[Frame];

	Tiles[Index]->SetFrame(TileFrame.Start, TileFrame.End);
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
	//auto Owner = this->Owner.lock();
	auto Shader = TileShader.lock();
	auto Mesh = TileMesh.lock();
	if (//!Owner ||
		!Shader || !Mesh)
	{
		return;
	}

	Shader->SetShader();

	Mesh->RenderInstancing(InstancingBuffer, InstancingCount);

	//auto World = this->World.lock();
	//if (!World)
	//{
	//	World = CWorldManager::GetInst()->GetWorld().lock();
	//}

	//auto CameraMgr = World->GetCameraManager().lock();
	//if (!CameraMgr)
	//{
	//	return;
	//}

	//for (int i = ViewStartY; i <= ViewEndY; i++)
	//{
	//	for (int j = ViewStartX; j <= ViewEndX; j++)
	//	{
	//		int Idx = i * CountX + j;

	//		const auto& Tile = Tiles[Idx];
	//		if (!Tile->GetRender())
	//		{
	//			continue;
	//		}

	//		FMatrix	ScaleMat, TranslateMat, WorldMat;

	//		ScaleMat.Scaling(TileSize);

	//		FVector2	Pos = Tile->GetPos();

	//		Pos.x += Owner->GetWorldPosition().x;
	//		Pos.y += Owner->GetWorldPosition().y;

	//		TranslateMat.Translation(Pos);

	//		WorldMat = ScaleMat * TranslateMat;

	//		Transform->SetWorldMatrix(WorldMat);

	//		Transform->SetViewMatrix(CameraMgr->GetViewMatrix());
	//		Transform->SetProjMatrix(CameraMgr->GetProjMatrix());

	//		Transform->UpdateBuffer();

	//		FVector2 LTUV = Tile->GetFrameStart() / TileTextureSize;
	//		FVector2 RBUV = Tile->GetFrameEnd() / TileTextureSize;

	//		CBufferTileMap->SetUV(LTUV, RBUV);

	//		CBufferTileMap->UpdateBuffer();

	//		Shader->SetShader();

	//		Mesh->Render();
	//	}
	//}
}

void CTileMapComponent::RenderTileOutLine()
{
	if (auto Shader = OutLineShader.lock())
	{
		Shader->SetShader();
	}

	if (auto Mesh = OutLineMesh.lock())
	{
		Mesh->RenderInstancing(LineInstancingBuffer, LineInstancingCount);
	}
}

void CTileMapComponent::CreateTile(ETileShape Shape, int CountX, int CountY, const FVector2& TileSize,
	int TileTextureFrame, bool bOutLineRender)
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

	// Disable this if tile map size is smaller than world.
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
			Tile->SetIndex(j, i, Index);

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
			Tile->SetOutlineRender(bOutLineRender);
		}
	}

	// Determines the maximum number of tiles that can be visible on the screen.
	FResolution	RS = CDevice::GetInst()->GetResolution();

	int	ViewCountX = RS.Width / TileSize.x + 3;
	int	ViewCountY = RS.Height / TileSize.y + 3;

	CreateInstancingBuffer(sizeof(FTileMapInstancingBuffer), ViewCountX * ViewCountY);
	CreateLineInstancingBuffer(sizeof(FTileMapInstancingBuffer), ViewCountX * ViewCountY);

	TileInstData.resize(ViewCountX * ViewCountY);
	TileLineInstData.resize(ViewCountX * ViewCountY);
}

bool CTileMapComponent::CreateInstancingBuffer(int Size, int Count)
{
	SAFE_RELEASE(InstancingBuffer.Buffer);

	InstancingBuffer.Size = Size;
	InstancingBuffer.Count = Count;

	D3D11_BUFFER_DESC BufferDesc{};
	BufferDesc.ByteWidth = Size * Count;
	BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;


	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&BufferDesc, nullptr, &InstancingBuffer.Buffer)))
	{
		return false;
	}

	return true;
}

bool CTileMapComponent::SetInstancingData(void* Data, int Count)
{
	if (!InstancingBuffer.Buffer)
	{
		return false;
	}

	if (InstancingBuffer.Count < Count)
	{
		if (!CreateInstancingBuffer(InstancingBuffer.Size, Count * 2))
		{
			return false;
		}
	}

	ID3D11DeviceContext* Context = CDevice::GetInst()->GetContext();

	D3D11_MAPPED_SUBRESOURCE MS{};

	Context->Map(InstancingBuffer.Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MS);

	memcpy(MS.pData, Data, InstancingBuffer.Size * Count);

	Context->Unmap(InstancingBuffer.Buffer, 0);

	InstancingCount = Count;

	return true;
}

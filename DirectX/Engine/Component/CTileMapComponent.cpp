#include "CTileMapComponent.h"

#include "CTile.h"
#include "CTileMapRender.h"
#include "../CDevice.h"
#include "../Asset/CAssetManager.h"
#include "../Asset/CPathManager.h"
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
		//TileShader = ShaderMgr->FindShader("TileMap");
		TileShader = ShaderMgr->FindShader("TileMapInstancing");
		OutlineShader = ShaderMgr->FindShader("TileMapLineInstancing");
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
			FVector2 LB, RT;

			LB.x = Center.x - RS.Width * 0.5f;
			LB.y = Center.y - RS.Height * 0.5f;

			RT.x = Center.x + RS.Width * 0.5f;
			RT.y = Center.y + RS.Height * 0.5f;

			ViewStartX = GetTileRenderIndexX(LB);
			ViewStartY = GetTileRenderIndexY(LB);

			ViewEndX = GetTileRenderIndexX(RT);
			ViewEndY = GetTileRenderIndexY(RT);

			ViewStartX -= 2;
			ViewEndX += 2;

			ViewStartY -= 2;
			ViewEndY += 2;
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
				FMatrix ScaleMat, TranslateMat, WorldMat;

				ScaleMat.Scaling(TileSize);

				FVector2 Pos = Tile->GetPos();
				Pos.x += Owner->GetWorldPosition().x;
				Pos.y += Owner->GetWorldPosition().y;

				TranslateMat.Translation(Pos);

				WorldMat = ScaleMat * TranslateMat;

				FMatrix ViewMat = CameraMgr->GetViewMatrix();
				FMatrix ProjMat = CameraMgr->GetProjMatrix();

				FMatrix WVPMat = WorldMat * ViewMat * ProjMat;

				auto& LineInstancingData = TileLineInstData[LineInstancingCount];

				LineInstancingData.WVP0 = WVPMat[0];
				LineInstancingData.WVP1 = WVPMat[1];
				LineInstancingData.WVP2 = WVPMat[2];
				LineInstancingData.WVP3 = WVPMat[3];

				LineInstancingData.Color = Tile->GetOutlineColor();

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
	//bRenderTileOutline = bRender;

	//if (bRenderTileOutline)
	//{

	//}
	//else
	//{
	//	OutlineMesh.reset();
	//	OutlineShader.reset();
	//}
}

void CTileMapComponent::SetTileOutlineRender(bool bRender, int Index)
{
	Tiles[Index]->SetOutlineRender(bRender);
}

void CTileMapComponent::SetOutlineMesh(const std::string& Key)
{
	if (auto World = this->World.lock())
	{
		if (auto AssetMgr = World->GetWorldAssetManager().lock())
		{
			OutlineMesh = AssetMgr->FindMesh(Key);
		}
	}
	else
	{
		if (auto MeshMgr = CAssetManager::GetInst()->GetMeshManager().lock())
		{
			OutlineMesh = MeshMgr->FindMesh("Mesh_" + Key);
		}
	}
}

void CTileMapComponent::SetOutlineShader(const std::string& Key)
{
	if (auto ShaderMgr = CAssetManager::GetInst()->GetShaderManager().lock())
	{
		OutlineShader = ShaderMgr->FindShader(Key);
	}
}

void CTileMapComponent::SetTileMesh(const std::string& Key)
{
	if (auto World = this->World.lock())
	{
		if (auto AssetMgr = World->GetWorldAssetManager().lock())
		{
			TileMesh = AssetMgr->FindMesh(Key);
		}
	}
	else
	{
		if (auto MeshMgr = CAssetManager::GetInst()->GetMeshManager().lock())
		{
			TileMesh = MeshMgr->FindMesh("Mesh_" + Key);
		}
	}
}

void CTileMapComponent::SetTileShader(const std::string& Key)
{
	if (auto ShaderMgr = CAssetManager::GetInst()->GetShaderManager().lock())
	{
		TileShader = ShaderMgr->FindShader(Key);
	}
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

int CTileMapComponent::GetTileRenderIndexX(const FVector2& Pos) const
{
	FVector WorldPosition = Owner.lock()->GetWorldPosition();

	switch (Shape)
	{
		case Rect:
		{
			// Calculates the relative position from the starting point of the tilemap.
			float Convert = Pos.x - WorldPosition.x;

			float Value = Convert / TileSize.x;

			int Index = (int)Value;

			if (Index < 0)
			{
				return 0;
			}
			else if (Index >= CountX)
			{
				return CountX - 1;
			}

			return Index;
		}
		case Isometric:
		{
			FVector2	ConvertPos;
			ConvertPos.x = Pos.x - WorldPosition.x;
			ConvertPos.y = Pos.y - WorldPosition.y;

			// It depends on whether the y index is odd or even.
			int	IndexY = GetTileRenderIndexY(ConvertPos);

			int	IndexX = -1;

			float Value = 0.f;

			// even
			if (IndexY % 2 == 0)
			{
				Value = ConvertPos.x / TileSize.x;
			}
			// odd
			else
			{
				Value = (ConvertPos.x - TileSize.x * 0.5f) / TileSize.x;
			}

			if (Value < 0.f)
			{
				return 0;
			}

			IndexX = (int)Value;

			if (IndexX >= CountX)
			{
				return CountX - 1;
			}

			return IndexX;
		}
	}

	return -1;
}

int CTileMapComponent::GetTileRenderIndexY(const FVector2& Pos) const
{
	switch (Shape)
	{
		case Rect:
		{
			// Calculate the relative position from the starting point of the tilemap.
			float Convert = Pos.y - Owner.lock()->GetWorldPosition().y;

			float Value = Convert / TileSize.y;

			int Index = (int)Value;

			if (Index < 0)
			{
				return 0;
			}
			else if (Index >= CountY)
			{
				return CountY - 1;
			}

			return Index;
		}
		case Isometric:
		{
			FVector2	ConvertPos;
			ConvertPos.x = Pos.x - Owner.lock()->GetWorldPosition().x;
			ConvertPos.y = Pos.y - Owner.lock()->GetWorldPosition().y;

			float RatioX = ConvertPos.x / TileSize.x;
			float RatioY = ConvertPos.y / TileSize.y;

			// Convert to index.
			int	IndexX = (int)RatioX;
			int	IndexY = (int)RatioY;

			if (IndexX < 0)
			{
				IndexX = 0;
			}
			else if (IndexX >= CountX)
			{
				IndexX = CountX - 1;
			}

			if (IndexY < 0)
			{
				IndexY = 0;
			}
			else if (IndexY >= CountY)
			{
				IndexY = CountY - 1;
			}

			// Subtract the integer part and leave only the decimal part.
			RatioX -= (int)RatioX;
			RatioY -= (int)RatioY;

			// If it exists in the lower part of the rectangular area
			if (RatioY < 0.5f)
			{
				// If it is the lower-left triangle of the lower-left rectangle
				// based on the rectangular area
				if (RatioY < 0.5f - RatioX)
				{
					// If it's the very bottom
					if (IndexY == 0)
					{
						return 0;
					}
					// If it's the very left
					else if (IndexX == 0)
					{
						// Only when the Y index is even, it doesn't exist.
						// However, since it needs to be drawn here, only exceptions
						// that are out of range are handled.
						if (IndexY < 0)
						{
							return 0;
						}
						else if (IndexY >= CountY)
						{
							return CountY - 1;
						}
					}
					return IndexY * 2 - 1;
				}

				// If it is the lower-right triangle of the lower-right rectangle
				// based on the rectangular area
				else if (RatioY < RatioX - 0.5f)
				{
					if (IndexY == 0)
					{
						return 0;
					}

					return IndexY * 2 - 1;
				}

				return IndexY * 2;
			}

			// If it exists in the upper part of the rectangular area
			else if (RatioY > 0.5f)
			{
				// If it is the upper-left triangle of the upper-left rectangle
				// based on the rectangular area
				if (RatioY - 0.5f > RatioX)
				{
					if (IndexX == 0)
					{
						if (IndexY < 0)
						{
							return 0;
						}
						else if (IndexY >= CountY)
						{
							return CountY - 1;
						}
					}
					else if (IndexY * 2 + 1 >= CountY)
					{
						return CountY - 1;
					}

					return IndexY * 2 + 1;
				}

				// If it is the upper-right triangle of the upper-right rectangle
				// based on the rectangular area
				else if (RatioY - 0.5f > 1.f - RatioX)
				{
					if (IndexX >= CountX)
					{
						if (IndexY < 0)
						{
							return 0;
						}
						else if (IndexY >= CountY)
						{
							return -1;
						}
					}
					else if (IndexY * 2 + 1 >= CountY)
					{
						return CountY - 1;
					}

					return IndexY * 2 + 1;
				}


				return IndexY * 2;
			}

			// If it exists in the center
			else
			{
				return IndexY * 2;
			}
		}
	}

	return -1;
}

ETileType CTileMapComponent::GetTileType(int Index) const
{
	return Tiles[Index]->GetType();
}

int CTileMapComponent::GetTileIndex(const FVector2& Pos) const
{
	auto Owner = this->Owner.lock();

	FVector OwnerPos = Owner->GetWorldPosition();
	FVector2 ConvertPos;

	ConvertPos.x = Pos.x - OwnerPos.x;
	ConvertPos.y = Pos.y - OwnerPos.y;

	int	IndexX = GetTileIndexX(ConvertPos);
	if (IndexX == -1)
	{
		return -1;
	}

	int	IndexY = GetTileIndexY(ConvertPos);
	if (IndexY == -1)
	{
		return -1;
	}

	return IndexY * CountX + IndexX;
}

int CTileMapComponent::GetTileIndex(const FVector& Pos) const
{
	return GetTileIndex(Pos.x, Pos.y);
}

int CTileMapComponent::GetTileIndex(float x, float y) const
{
	return GetTileIndex(FVector2(x, y));
}

int CTileMapComponent::GetTileIndexX(const FVector2& Pos) const
{
	switch (Shape)
	{
		case Rect:
		{
			// Calculates the relative position from the starting point of the tile map.
			float Convert = Pos.x - this->Owner.lock()->GetWorldPosition().x;

			float Value = Convert / TileSize.x;

			int Index = (int)Value;

			if (Index < 0 || Index >= CountX)
				return -1;

			return Index;
		}
		case Isometric:
		{
			FVector2 ConvertPos;
			ConvertPos.x = Pos.x - this->Owner.lock()->GetWorldPosition().x;
			ConvertPos.y = Pos.y - this->Owner.lock()->GetWorldPosition().y;

			int	IndexY = GetTileIndexY(ConvertPos);

			int	IndexX = -1;

			float Value = 0.f;

			// even
			if (IndexY % 2 == 0)
			{
				Value = ConvertPos.x / TileSize.x;
			}
			// odd
			else
			{
				Value = (ConvertPos.x - TileSize.x * 0.5f) / TileSize.x;
			}

			if (Value < 0.f)
			{
				return -1;
			}

			IndexX = (int)Value;
			if (IndexX >= CountX)
			{
				return -1;
			}

			return IndexX;
		}
	}

	return -1;
}

int CTileMapComponent::GetTileIndexY(const FVector2& Pos) const
{
	switch (Shape)
	{
		case Rect:
		{
			float Convert = Pos.y - this->Owner.lock()->GetWorldPosition().y;

			float Value = Convert / TileSize.y;

			int Index = (int)Value;

			if (Index < 0 || Index >= CountY)
			{
				return -1;
			}

			return Index;
		}
		case Isometric:
		{
			FVector2 ConvertPos;
			ConvertPos.x = Pos.x - this->Owner.lock()->GetWorldPosition().x;
			ConvertPos.y = Pos.y - this->Owner.lock()->GetWorldPosition().y;

			if (ConvertPos.x < 0.f || ConvertPos.x > MapSize.x
				|| ConvertPos.y < 0.f || ConvertPos.y > MapSize.y)
			{
				return -1;
			}

			float RatioX = ConvertPos.x / TileSize.x;
			float RatioY = ConvertPos.y / TileSize.y;

			int	IndexX = (int)RatioX;
			int	IndexY = (int)RatioY;

			RatioX -= (int)RatioX;
			RatioY -= (int)RatioY;

			if (RatioY < 0.5f)
			{
				if (RatioY < 0.5f - RatioX)
				{
					if (IndexY == 0)
					{
						return -1;
					}
					else if (IndexX == 0)
					{
						if (IndexY < 0)
						{
							return -1;
						}
						else if (IndexX == 0)
						{
							return -1;
						}
						else if (IndexY >= CountY / 2 + 1)
						{
							return -1;
						}
					}

					return IndexY * 2 - 1;
				}
				else if (RatioY < RatioX - 0.5f)
				{
					if (IndexY == 0)
					{
						return -1;
					}
					else if (IndexY >= CountY / 2 + 1)
					{
						return -1;
					}

					return IndexY * 2 - 1;
				}
				else if (IndexY >= CountY / 2)
				{
					return -1;
				}

				return IndexY * 2;
			}
			else if (RatioY > 0.5f)
			{
				if (RatioY - 0.5f > RatioX)
				{
					if (IndexX == 0)
					{
						return -1;
					}
					else if (IndexY * 2 + 1 >= CountY)
					{
						return -1;
					}
					else if (IndexY >= CountY / 2)
					{
						return -1;
					}

					return IndexY * 2 + 1;
				}
				else if (RatioY - 0.5f > 1.f - RatioX)
				{
					if (IndexX >= CountX)
					{
						return -1;
					}
					else if (IndexY * 2 + 1 >= CountY)
					{
						return -1;
					}
					else if (IndexY >= CountY / 2)
					{
						return -1;
					}

					return IndexY * 2 + 1;
				}
				else if (IndexY >= CountY / 2)
				{
					return -1;
				}

				return IndexY * 2;
			}
			else
			{
				if (IndexY >= CountY / 2)
				{
					return -1;
				}

				return IndexY * 2;
			}
		}
	}

	return -1;
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
	if (auto Shader = OutlineShader.lock())
	{
		Shader->SetShader();
	}

	if (auto Mesh = OutlineMesh.lock())
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
			MapSize = TileSize * FVector2(static_cast<float>(CountX), static_cast<float>(CountY));
			if (auto World = this->World.lock())
			{
				if (auto AssetMgr = World->GetWorldAssetManager().lock())
				{
					OutlineMesh = AssetMgr->FindMesh("LBFrameRect");
				}
			}
			else
			{
				if (auto MeshMgr = CAssetManager::GetInst()->GetMeshManager().lock())
				{
					OutlineMesh = MeshMgr->FindMesh("Mesh_LBFrameRect");
				}
			}
			break;
		case Isometric:
			MapSize.x = TileSize.x * CountX + TileSize.x * 0.5f;
			MapSize.y = TileSize.y + TileSize.y * 0.5f * (CountY - 1);
			if (auto World = this->World.lock())
			{
				if (auto AssetMgr = World->GetWorldAssetManager().lock())
				{
					OutlineMesh = AssetMgr->FindMesh("IstFrameRect");
				}
			}
			else
			{
				if (auto MeshMgr = CAssetManager::GetInst()->GetMeshManager().lock())
				{
					OutlineMesh = MeshMgr->FindMesh("Mesh_IstFrameRect");
				}
			}
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
					if (i % 2 == 0)
					{
						Tile->SetPos(j * TileSize.x, i * TileSize.y * 0.5f);
					}
					else
					{
						Tile->SetPos(j * TileSize.x + TileSize.x * 0.5f, i * TileSize.y * 0.5f);
					}
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

	int	ViewCountX = static_cast<int>(RS.Width / TileSize.x + 5);
	int	ViewCountY = static_cast<int>(RS.Height / TileSize.y + 3);

	if (Shape == Isometric)
	{
		ViewCountY = ViewCountY * 2 + 2;
	}

	CreateInstancingBuffer(sizeof(FTileMapInstancingBuffer), ViewCountX * ViewCountY);
	CreateLineInstancingBuffer(sizeof(FTileMapInstancingBuffer), ViewCountX * ViewCountY);

	TileInstData.resize(ViewCountX * ViewCountY);
	TileLineInstData.resize(ViewCountX * ViewCountY);
}

void CTileMapComponent::Save(const TCHAR* FileName, const std::string& PathName)
{
	TCHAR	FullPath[MAX_PATH] = {};

	lstrcpy(FullPath, CPathManager::FindPath(PathName));
	lstrcat(FullPath, FileName);

	SaveFullPath(FullPath);
}

void CTileMapComponent::SaveFullPath(const TCHAR* FullPath)
{
	char	FullPathMultibyte[MAX_PATH] = {};

	int Length = WideCharToMultiByte(CP_ACP, 0, FullPath,
		-1, nullptr, 0, nullptr, nullptr);
	WideCharToMultiByte(CP_ACP, 0, FullPath, -1,
		FullPathMultibyte, Length, nullptr, nullptr);

	FILE* File = nullptr;

	fopen_s(&File, FullPathMultibyte, "wb");

	if (!File)
	{
		return;
	}

	if (auto Render = TileMapRender.lock())
	{
		Render->Save(File);
	}

	size_t Size = Tiles.size();

	fwrite(&Size, sizeof(size_t), 1, File);

	for (const auto& Tile : Tiles)
	{
		Tile->Save(File);
	}

	fwrite(&Shape, sizeof(ETileShape), 1, File);
	fwrite(&TileSize, sizeof(FVector2), 1, File);
	fwrite(&MapSize, sizeof(FVector2), 1, File);

	fwrite(&CountX, sizeof(int), 1, File);
	fwrite(&CountY, sizeof(int), 1, File);

	fwrite(&bRenderTileOutline, sizeof(bool), 1, File);

	auto OutlineMesh = this->OutlineMesh.lock();

	bool Enable = false;

	if (OutlineMesh)
	{
		Enable = true;
	}

	fwrite(&Enable, sizeof(bool), 1, File);

	if (OutlineMesh)
	{
		std::string	Key = OutlineMesh->GetSplitKey();
		size_t Count = Key.length();

		fwrite(&Count, sizeof(size_t), 1, File);
		fwrite(Key.c_str(), 1, Key.length(), File);
	}

	auto OutlineShader = this->OutlineShader.lock();

	Enable = false;

	if (OutlineShader)
	{
		Enable = true;
	}

	fwrite(&Enable, sizeof(bool), 1, File);

	if (OutlineShader)
	{
		std::string	Key = OutlineShader->GetKey();
		size_t Count = Key.length();

		fwrite(&Count, sizeof(size_t), 1, File);
		fwrite(Key.c_str(), 1, Key.length(), File);
	}

	auto TileMesh = this->TileMesh.lock();

	Enable = false;

	if (TileMesh)
	{
		Enable = true;
	}

	fwrite(&Enable, sizeof(bool), 1, File);

	if (TileMesh)
	{
		std::string	Key = TileMesh->GetSplitKey();
		size_t Count = Key.length();

		fwrite(&Count, sizeof(size_t), 1, File);
		fwrite(Key.c_str(), 1, Key.length(), File);
	}

	auto TileShader = this->TileShader.lock();

	Enable = false;

	if (TileShader)
	{
		Enable = true;
	}

	fwrite(&Enable, sizeof(bool), 1, File);

	if (TileShader)
	{
		std::string	Key = TileShader->GetKey();
		size_t Count = Key.length();

		fwrite(&Count, sizeof(size_t), 1, File);
		fwrite(Key.c_str(), 1, Key.length(), File);
	}

	fwrite(&TileTextureSize, sizeof(FVector2), 1, File);

	Size = TileFrames.size();

	fwrite(&Size, sizeof(size_t), 1, File);

	fwrite(&TileFrames[0], sizeof(FTileFrame), Size, File);

	fclose(File);
}

void CTileMapComponent::Load(const TCHAR* FileName, const std::string& PathName)
{
	TCHAR FullPath[MAX_PATH] = {};

	lstrcpy(FullPath, CPathManager::FindPath(PathName));
	lstrcat(FullPath, FileName);

	LoadFullPath(FullPath);
}

void CTileMapComponent::LoadFullPath(const TCHAR* FullPath)
{
	char FullPathMultibyte[MAX_PATH] = {};

	int Length = WideCharToMultiByte(CP_ACP, 0, FullPath, -1, nullptr, 0, nullptr, nullptr);
	WideCharToMultiByte(CP_ACP, 0, FullPath, -1,
		FullPathMultibyte, Length, nullptr, nullptr);

	FILE* File = nullptr;

	fopen_s(&File, FullPathMultibyte, "rb");

	if (!File)
	{
		return;
	}

	if (auto Render = TileMapRender.lock())
	{
		Render->Load(File);
	}

	size_t Size = 0;

	fread(&Size, sizeof(size_t), 1, File);

	Tiles.clear();
	Tiles.resize(Size);

	for (auto& Tile : Tiles)
	{
		Tile.reset(new CTile);
		Tile->Load(File);
	}

	FResolution	RS = CDevice::GetInst()->GetResolution();

	int	ViewCountX = (int)(RS.Width / TileSize.x + 3);
	int	ViewCountY = (int)(RS.Height / TileSize.y + 3);

	if (Shape == Isometric)
	{
		ViewCountY = ViewCountY * 2 + 2;
	}

	CreateInstancingBuffer(sizeof(FTileMapInstancingBuffer), ViewCountX * ViewCountY);

	CreateLineInstancingBuffer(sizeof(FTileMapInstancingBuffer), ViewCountX * ViewCountY);

	TileInstData.clear();
	TileLineInstData.clear();

	TileInstData.resize(ViewCountX * ViewCountY);
	TileLineInstData.resize(ViewCountX * ViewCountY);

	fread(&Shape, sizeof(ETileShape), 1, File);
	fread(&TileSize, sizeof(FVector2), 1, File);
	fread(&MapSize, sizeof(FVector2), 1, File);

	fread(&CountX, sizeof(int), 1, File);
	fread(&CountY, sizeof(int), 1, File);

	fread(&bRenderTileOutline, sizeof(bool), 1, File);

	bool Enable = false;

	fread(&Enable, sizeof(bool), 1, File);

	if (Enable)
	{
		char Key[256] = {};

		size_t	Count = 0;

		fread(&Count, sizeof(size_t), 1, File);

		fread(Key, 1, Count, File);

		SetOutlineMesh(Key);
	}

	Enable = false;

	fread(&Enable, sizeof(bool), 1, File);

	if (Enable)
	{
		char Key[256] = {};

		size_t	Count = 0;

		fread(&Count, sizeof(size_t), 1, File);

		fread(Key, 1, Count, File);

		SetOutlineShader(Key);
	}

	Enable = false;

	fread(&Enable, sizeof(bool), 1, File);

	if (Enable)
	{
		char Key[256] = {};

		size_t	Count = 0;

		fread(&Count, sizeof(size_t), 1, File);

		fread(Key, 1, Count, File);

		SetTileMesh(Key);
	}

	Enable = false;

	fread(&Enable, sizeof(bool), 1, File);

	if (Enable)
	{
		char Key[256] = {};

		size_t	Count = 0;

		fread(&Count, sizeof(size_t), 1, File);

		fread(Key, 1, Count, File);

		SetTileShader(Key);
	}

	fread(&TileTextureSize, sizeof(FVector2), 1, File);

	Size = 0;

	fread(&Size, sizeof(size_t), 1, File);

	TileFrames.clear();
	TileFrames.resize(Size);

	fread(&TileFrames[0], sizeof(FTileFrame), Size, File);

	fclose(File);
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

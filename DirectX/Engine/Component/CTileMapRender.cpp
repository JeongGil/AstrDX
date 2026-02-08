#include "CTileMapRender.h"

#include "CTileMapComponent.h"
#include "../Asset/CAssetManager.h"
#include "../Asset/Mesh/CMeshManager.h"
#include "../Asset/Shader/CShaderManager.h"
#include "../Asset/Texture/CTexture.h"
#include "../Asset/Texture/CTextureManager.h"
#include "../Render/CRenderManager.h"
#include "../World/CWorld.h"
#include "../Asset/Shader/CCBufferTransform.h"
#include "../World/CWorldManager.h"
#include "../Render/CRenderState.h"

CTileMapRender::CTileMapRender()
{
	RenderType = EComponentRender::Render;
}

CTileMapRender::CTileMapRender(const CTileMapRender& other) :
	CSceneComponent(other),
	BackShader(other.BackShader),
	BackMesh(other.BackMesh),
	AlphaBlend(other.AlphaBlend),
	TileAlphaBlend(other.TileAlphaBlend),
	TransformCBuffer(other.TransformCBuffer),
	TileMap(other.TileMap),
	TileFrames(other.TileFrames)
{
}

CTileMapRender::CTileMapRender(CTileMapRender&& other) noexcept :
	CSceneComponent(std::move(other)),
	BackShader(std::move(other.BackShader)),
	BackMesh(std::move(other.BackMesh)),
	AlphaBlend(std::move(other.AlphaBlend)),
	TileAlphaBlend(std::move(other.TileAlphaBlend)),
	TransformCBuffer(std::move(other.TransformCBuffer)),
	TileMap(std::move(other.TileMap)),
	TileFrames(std::move(other.TileFrames))
{
}

CTileMapRender::~CTileMapRender()
{
}

bool CTileMapRender::Init()
{
	SetRenderLayer("Map");

	if (!CSceneComponent::Init())
	{
		return false;
	}

	if (TileMap.expired())
	{
		auto Owner = this->Owner.lock();

		this->TileMap = Owner->GetComponent<CTileMapComponent>();
	}

	if (auto TileMap = this->TileMap.lock())
	{
		SetWorldScale(TileMap->GetMapSize());
	}

	TransformCBuffer.reset(new CCBufferTransform);
	TransformCBuffer->Init();

	SetBackShader("DefaultTex");
	SetBackMesh("RectTex");

	return true;
}

void CTileMapRender::Render()
{
	CSceneComponent::Render();

	if (!Textures[ETileTextureType::Back].expired())
	{
		FMatrix	ScaleMat, TranslateMat;

		ScaleMat.Scaling(WorldScale);
		TranslateMat.Translation(WorldPosition);

		FMatrix WorldMat = ScaleMat * TranslateMat;

		TransformCBuffer->SetWorldMatrix(WorldMat);

		auto World = this->World.lock();

		if (!World)
		{
			World = CWorldManager::GetInst()->GetWorld().lock();
		}

		auto CameraMgr = World->GetCameraManager().lock();

		TransformCBuffer->SetViewMatrix(CameraMgr->GetViewMatrix());
		TransformCBuffer->SetProjMatrix(CameraMgr->GetProjMatrix());

		auto Shader = BackShader.lock();
		auto Mesh = BackMesh.lock();

		FVector PivotSize = Pivot * Mesh->GetMeshSize();

		TransformCBuffer->SetPivotSize(PivotSize);

		TransformCBuffer->UpdateBuffer();

		auto	Texture = Textures[ETileTextureType::Back].lock();

		Texture->SetShader(0, EShaderBufferType::Pixel, 0);

		auto State = AlphaBlend.lock();

		if (State)
		{
			State->SetState();
		}

		Shader->SetShader();

		Mesh->Render();

		if (State)
		{
			State->ResetState();
		}
	}

	// 타일 출력
	if (auto TileMap = this->TileMap.lock())
	{
		if (auto Texture = Textures[ETileTextureType::Tile].lock())
		{
			Texture->SetShader(0, EShaderBufferType::Pixel,				0);

			auto State = TileAlphaBlend.lock();

			if (State)
			{
				State->SetState();
			}

			TileMap->RenderTile();

			if (State)
			{
				State->ResetState();
			}
		}

		TileMap->RenderTileOutLine();
	}
}

CTileMapRender* CTileMapRender::Clone() const
{
	return new CTileMapRender(*this);
}

void CTileMapRender::SetTileMapComponent(const std::weak_ptr<CTileMapComponent>& TileMap)
{
	this->TileMap = TileMap;

	if (auto TileMapComp = TileMap.lock())
	{
		SetWorldScale(TileMapComp->GetMapSize());
	}
}

void CTileMapRender::EnableAlphaBlend()
{
	AlphaBlend = CRenderManager::GetInst()->FindRenderState("AlphaBlend");
}

void CTileMapRender::EnableTileAlphaBlend()
{
	TileAlphaBlend = CRenderManager::GetInst()->FindRenderState("AlphaBlend");
}

bool CTileMapRender::SetTexture(ETileTextureType::Type Type, const std::weak_ptr<CTexture>& Texture)
{
	Textures[Type] = Texture;

	if (Type == ETileTextureType::Tile)
	{
		auto Tex = this->Textures[Type].lock();
		auto TileMap = this->TileMap.lock();;
		if (Tex && TileMap)
		{
			TileMap->SetTileTextureSize(static_cast<float>(Tex->GetTexture()->Width), static_cast<float>(Tex->GetTexture()->Height));
		}
	}

	return true;
}

bool CTileMapRender::SetTexture(ETileTextureType::Type Type, const std::string& Key)
{
	auto& Texture = Textures[Type];
	if (auto World = this->World.lock())
	{
		if (auto AssetMgr = World->GetWorldAssetManager().lock())
		{
			Texture = AssetMgr->FindTexture(Key);
		}
	}
	else
	{
		if (auto TexMgr = CAssetManager::GetInst()->GetTextureManager().lock())
		{
			Texture = TexMgr->FindTexture(Key);
		}
	}

	SetTexture(Type, Texture);
}

bool CTileMapRender::SetTexture(ETileTextureType::Type Type, const std::string& Key, const TCHAR* FileName,
	const std::string& PathName)
{
	auto& Texture = Textures[Type];
	if (auto World = this->World.lock())
	{
		if (auto AssetMgr = World->GetWorldAssetManager().lock())
		{
			if (!AssetMgr->LoadTexture(Key, FileName, PathName))
			{
				return false;
			}

			Texture = AssetMgr->FindTexture(Key);
		}
	}
	else
	{
		if (auto TexMgr = CAssetManager::GetInst()->GetTextureManager().lock())
		{
			if (!TexMgr->LoadTexture("Texture_" + Key, FileName, PathName))
			{
				return false;
			}

			Texture = TexMgr->FindTexture(Key);
		}
	}

	SetTexture(Type, Texture);
}

void CTileMapRender::SetBackMesh(const std::weak_ptr<CMesh>& Mesh)
{
	BackMesh = Mesh;
}

void CTileMapRender::SetBackMesh(const std::string& Key)
{
	if (auto World = this->World.lock())
	{
		if (auto AssetMgr = World->GetWorldAssetManager().lock())
		{
			BackMesh = AssetMgr->FindMesh(Key);
		}
	}

	else
	{
		if (auto MeshMgr = CAssetManager::GetInst()->GetMeshManager().lock())
		{
			BackMesh = MeshMgr->FindMesh("Mesh_" + Key);
		}
	}
}

void CTileMapRender::SetBackShader(const std::weak_ptr<CShader>& Shader)
{
	BackShader = Shader;
}

void CTileMapRender::SetBackShader(const std::string& Key)
{
	if (auto ShaderMgr = CAssetManager::GetInst()->GetShaderManager().lock())
	{
		SetBackShader(ShaderMgr->FindShader(Key));
	}
}

void CTileMapRender::SetTileFrameSize(float x, float y)
{
	if (auto TileMap = this->TileMap.lock())
	{

	}
}

void CTileMapRender::AddTileFrame(const FVector2& Start, const FVector2& End)
{
	TileFrames.emplace_back(Start, End);

	if (auto TileMap = this->TileMap.lock())
	{
		TileMap->AddTileFrame(Start, End);
	}
}

void CTileMapRender::AddTileFrame(float StartX, float StartY, float EndX, float EndY)
{
	AddTileFrame(FVector2(StartX, StartY), FVector2(EndX, EndY));
}

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

	if (auto Texture = Textures[ETileTextureType::Back].lock())
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
			Texture->SetShader(1, EShaderBufferType::Pixel, 0);

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
		TileMapComp->SetTileMapRender(std::dynamic_pointer_cast<CTileMapRender>(shared_from_this()));

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

	return SetTexture(Type, Texture);
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

	return SetTexture(Type, Texture);
}

bool CTileMapRender::SetTextureFullPath(ETileTextureType::Type Type, const std::string& Key, const TCHAR* FullPath)
{
	auto& WeakTexture = Textures[Type];

	if (auto World = this->World.lock())
	{
		auto AssetMgr = World->GetWorldAssetManager().lock();

		if (!AssetMgr->LoadTextureFullPath(Key, FullPath))
		{
			return false;
		}

		WeakTexture = AssetMgr->FindTexture(Key);
	}
	else
	{
		auto TexMgr = CAssetManager::GetInst()->GetTextureManager().lock();

		auto InnerKey = "Texture_" + Key;
		if (!TexMgr->LoadTextureFullPath(InnerKey, FullPath))
		{
			return false;
		}

		WeakTexture = TexMgr->FindTexture(InnerKey);
	}

	if (Type == ETileTextureType::Tile)
	{
		auto Texture = WeakTexture.lock();

		if (auto TileMap = this->TileMap.lock())
		{
			TileMap->SetTileTextureSize((float)Texture->GetTexture()->Width, (float)Texture->GetTexture()->Height);
		}
	}

	return true;
}

bool CTileMapRender::SetTextureFullPath(ETileTextureType::Type Type, const std::string& Key,
	const std::vector<const TCHAR*>& FullPaths)
{
	auto& WeakTexture = Textures[Type];

	if (auto World = this->World.lock())
	{
		auto AssetMgr = World->GetWorldAssetManager().lock();

		if (!AssetMgr->LoadTexturesFullPath(Key, FullPaths))
		{
			return false;
		}

		WeakTexture = AssetMgr->FindTexture(Key);
	}
	else
	{
		auto TexMgr = CAssetManager::GetInst()->GetTextureManager().lock();

		auto InnerKey = "Texture_" + Key;
		if (!TexMgr->LoadTexturesFullPath(InnerKey, FullPaths))
		{
			return false;
		}

		WeakTexture = TexMgr->FindTexture(InnerKey);
	}

	if (Type == ETileTextureType::Tile)
	{
		auto Texture = WeakTexture.lock();

		if (auto TileMap = this->TileMap.lock())
		{
			TileMap->SetTileTextureSize((float)Texture->GetTexture()->Width, (float)Texture->GetTexture()->Height);
		}
	}

	return true;
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

void CTileMapRender::Save(FILE* File)
{
	auto BackShader = this->BackShader.lock();

	bool Enable = false;

	if (BackShader)
	{
		Enable = true;
	}

	fwrite(&Enable, sizeof(bool), 1, File);

	if (BackShader)
	{
		std::string	Key = BackShader->GetKey();
		size_t Count = Key.length();

		fwrite(&Count, sizeof(size_t), 1, File);
		fwrite(Key.c_str(), 1, Key.length(), File);
	}

	auto BackMesh = this->BackMesh.lock();

	Enable = false;

	if (BackMesh)
	{
		Enable = true;
	}

	fwrite(&Enable, sizeof(bool), 1, File);

	if (BackMesh)
	{
		std::string	Key = BackMesh->GetSplitKey();
		size_t Count = Key.length();

		fwrite(&Count, sizeof(size_t), 1, File);
		fwrite(Key.c_str(), 1, Key.length(), File);
	}

	for (const auto& WeakTexture : Textures)
	{
		auto Texture = WeakTexture.lock();

		Enable = false;

		if (Texture)
		{
			Enable = true;
		}

		fwrite(&Enable, sizeof(bool), 1, File);

		if (Texture)
		{
			std::string Key = Texture->GetSplitKey();
			size_t Count = Key.length();

			fwrite(&Count, sizeof(size_t), 1, File);
			fwrite(Key.c_str(), 1, Key.length(), File);

			Texture->Save(File);
		}
	}

	auto Blend = AlphaBlend.lock();

	Enable = false;

	if (Blend)
	{
		Enable = true;
	}

	fwrite(&Enable, sizeof(bool), 1, File);

	auto TileBlend = TileAlphaBlend.lock();

	Enable = false;

	if (TileBlend)
	{
		Enable = true;
	}

	fwrite(&Enable, sizeof(bool), 1, File);

	size_t Size = TileFrames.size();

	fwrite(&Size, sizeof(size_t), 1, File);

	fwrite(&TileFrames[0], sizeof(FTileFrame), Size, File);

	fwrite(&WorldScale, sizeof(FVector3), 1, File);
	fwrite(&WorldRotation, sizeof(FVector3), 1, File);
	fwrite(&WorldPosition, sizeof(FVector3), 1, File);
	fwrite(WorldAxis, sizeof(FVector3), EAxis::End, File);
}

void CTileMapRender::Load(FILE* File)
{
	bool Enable = false;

	fread(&Enable, sizeof(bool), 1, File);

	if (Enable)
	{
		char Name[256] = {};

		size_t Count = 0;

		fread(&Count, sizeof(size_t), 1, File);

		fread(Name, 1, Count, File);

		SetBackShader(Name);
	}

	Enable = false;

	fread(&Enable, sizeof(bool), 1, File);

	if (Enable)
	{
		char Name[256] = {};

		size_t Count = 0;

		fread(&Count, sizeof(size_t), 1, File);

		fread(Name, 1, Count, File);

		SetBackMesh(Name);
	}

	for (int i = 0; i < ETileTextureType::End; ++i)
	{
		Enable = false;

		fread(&Enable, sizeof(bool), 1, File);

		if (Enable)
		{
			char Name[256] = {};

			size_t	Count = 0;

			fread(&Count, sizeof(size_t), 1, File);

			fread(Name, 1, Count, File);

			// Get tex full path.
			size_t	TexCount = 0;
			fread(&TexCount, sizeof(size_t), 1, File);

			if (TexCount > 1)
			{
				std::vector<const TCHAR*>	FullPaths;

				for (size_t j = 0; j < TexCount; ++j)
				{
					size_t PathCount = 0;

					TCHAR* FullPath = new TCHAR[MAX_PATH];
					memset(FullPath, 0, sizeof(TCHAR) * MAX_PATH);

					fread(&PathCount, sizeof(size_t), 1, File);
					fread(FullPath, sizeof(wchar_t), PathCount, File);

					FullPaths.push_back(FullPath);
				}

				SetTextureFullPath((ETileTextureType::Type)i, Name, FullPaths);

				for (auto& FullPath : FullPaths)
				{
					SAFE_DELETE_ARRAY(FullPath);
				}
			}
			else
			{
				size_t PathCount = 0;
				TCHAR FullPath[MAX_PATH] = {};

				fread(&PathCount, sizeof(size_t), 1, File);
				fread(FullPath, sizeof(wchar_t), PathCount, File);

				SetTextureFullPath((ETileTextureType::Type)i, Name, FullPath);
			}
		}
	}

	Enable = false;

	fread(&Enable, sizeof(bool), 1, File);

	if (Enable)
	{
		EnableAlphaBlend();
	}

	Enable = false;

	fread(&Enable, sizeof(bool), 1, File);

	if (Enable)
	{
		EnableTileAlphaBlend();
	}

	size_t	Size = 0;

	fread(&Size, sizeof(size_t), 1, File);

	TileFrames.clear();

	TileFrames.resize(Size);

	fread(&TileFrames[0], sizeof(FTileFrame), Size, File);

	fread(&WorldScale, sizeof(FVector3), 1, File);
	fread(&WorldRotation, sizeof(FVector3), 1, File);
	fread(&WorldPosition, sizeof(FVector3), 1, File);
	fread(WorldAxis, sizeof(FVector3), EAxis::End, File);

	SetWorldPosition(WorldPosition);
	SetWorldScale(WorldScale);
	SetWorldRotation(WorldRotation);
}

#include "CWorldAssetManager.h"

#include "../Asset/CAssetManager.h"
#include "../Asset/Animation2D/CAnimation2DManager.h"
#include "../Asset/Mesh/CMeshManager.h"
#include "../Asset/Texture/CTextureManager.h"

bool CWorldAssetManager::CreateMesh(const std::string& Key, void* Vertices, int VertexSize, int VertexCount,
	D3D11_USAGE VertexUsage, D3D11_PRIMITIVE_TOPOLOGY Topology, void* Indices, int IndexSize, int IndexCount,
	DXGI_FORMAT Format, D3D11_USAGE IndexUsage)
{
	auto MeshMgr = CAssetManager::GetInst()->GetMeshManager().lock();
	if (!MeshMgr)
	{
		return false;
	}

	auto InnerKey = "Mesh_" + Key;
	if (!MeshMgr->CreateMesh(InnerKey, Vertices, VertexSize, VertexCount, VertexUsage, Topology, Indices,
		IndexSize, IndexCount, Format, IndexUsage))
	{
		return false;
	}

	Assets.try_emplace(InnerKey, MeshMgr->FindMesh(InnerKey));

	return true;
}

std::weak_ptr<CMesh> CWorldAssetManager::FindMesh(const std::string& Key)
{
	auto InnerKey = "Mesh_" + Key;

	if (!Assets.contains(InnerKey))
	{
		if (auto MeshMgr = CAssetManager::GetInst()->GetMeshManager().lock())
		{
			if (auto Mesh = MeshMgr->FindMesh(InnerKey).lock())
			{
				Assets.emplace(InnerKey, Mesh);
			}
			else
			{
				return std::weak_ptr<CMesh>();
			}
		}
	}

	return std::dynamic_pointer_cast<CMesh>(Assets[InnerKey]);
}

bool CWorldAssetManager::LoadTexture(const std::string& Key, const TCHAR* FileName, const std::string& PathName)
{
	auto TexMgr = CAssetManager::GetInst()->GetTextureManager().lock();
	if (!TexMgr)
	{
		return false;
	}

	auto InnerKey = "Texture_" + Key;
	if (!TexMgr->LoadTexture(InnerKey, FileName, PathName))
	{
		return false;
	}

	Assets.try_emplace(InnerKey, TexMgr->FindTexture(InnerKey));

	return true;
}

bool CWorldAssetManager::LoadTextureFullPath(const std::string& Key, const TCHAR* FullPath)
{
	auto TexMgr = CAssetManager::GetInst()->GetTextureManager().lock();
	if (!TexMgr)
	{
		return false;
	}

	auto InnerKey = "Texture_" + Key;
	if (!TexMgr->LoadTextureFullPath(InnerKey, FullPath))
	{
		return false;
	}

	Assets.try_emplace(InnerKey, TexMgr->FindTexture(InnerKey));

	return true;
}

bool CWorldAssetManager::LoadTextures(const std::string& Key, const std::vector<const TCHAR*>& FileNames,
	const std::string& PathName)
{
	auto TexMgr = CAssetManager::GetInst()->GetTextureManager().lock();
	if (!TexMgr)
	{
		return false;
	}

	auto InnerKey = "Texture_" + Key;
	if (!TexMgr->LoadTextures(InnerKey, FileNames, PathName))
	{
		return false;
	}

	Assets.try_emplace(InnerKey, TexMgr->FindTexture(InnerKey));

	return true;
}

bool CWorldAssetManager::LoadTexturesFullPath(const std::string& Key, const std::vector<const TCHAR*>& FullPaths)
{
	auto TexMgr = CAssetManager::GetInst()->GetTextureManager().lock();
	if (!TexMgr)
	{
		return false;
	}

	auto InnerKey = "Texture_" + Key;
	if (!TexMgr->LoadTexturesFullPath(InnerKey, FullPaths))
	{
		return false;
	}

	Assets.try_emplace(InnerKey, TexMgr->FindTexture(InnerKey));

	return true;
}

std::weak_ptr<CTexture> CWorldAssetManager::FindTexture(const std::string& Key)
{
	auto InnerKey = "Texture_" + Key;

	if (!Assets.contains(InnerKey))
	{
		if (auto TexMgr = CAssetManager::GetInst()->GetTextureManager().lock())
		{
			if (auto Mesh = TexMgr->FindTexture(InnerKey).lock())
			{
				Assets.emplace(InnerKey, Mesh);
			}
			else
			{
				return std::weak_ptr<CTexture>();
			}
		}
	}

	return std::dynamic_pointer_cast<CTexture>(Assets[InnerKey]);
}

bool CWorldAssetManager::CreateAnimation(const std::string& AnimKey)
{
	auto Mgr = CAssetManager::GetInst()->GetAnimation2DManager().lock();
	if (!Mgr)
	{
		return false;
	}

	auto InnerKey = "Animation2D_" + AnimKey;
	if (!Mgr->CreateAnimation(InnerKey))
	{
		return false;
	}

	Assets.try_emplace(InnerKey, Mgr->FindAnimation(InnerKey));

	return true;
}

std::weak_ptr<CAnimation2D> CWorldAssetManager::FindAnimation(const std::string& AnimKey)
{
	auto InnerKey = "Animation2D_" + AnimKey;

	if (auto Mgr = CAssetManager::GetInst()->GetAnimation2DManager().lock())
	{
		if (!Assets.contains(InnerKey))
		{
			if (auto Anim = Mgr->FindAnimation(InnerKey).lock())
			{
				Assets.emplace(InnerKey, Anim);
			}
			else
			{
				return std::weak_ptr<CAnimation2D>();
			}
		}
	}

	return std::dynamic_pointer_cast<CAnimation2D>(Assets[InnerKey]);
}

bool CWorldAssetManager::SetAnimation2DTextureType(const std::string& AnimKey, EAnimation2DTextureType TextureType)
{
	auto InnerKey = "Animation2D_" + AnimKey;

	auto It = Assets.find(InnerKey);
	if (It == Assets.end())
	{
		return false;
	}

	auto Anim = std::dynamic_pointer_cast<CAnimation2D>(It->second);
	if (!Anim)
	{
		return false;
	}

	Anim->SetTextureType(TextureType);

	return true;
}

bool CWorldAssetManager::SetTexture(const std::string& AnimKey, const std::weak_ptr<CTexture>& Texture)
{
	auto InnerKey = "Animation2D_" + AnimKey;

	auto It = Assets.find(InnerKey);
	if (It == Assets.end())
	{
		return false;
	}

	auto Anim = std::dynamic_pointer_cast<CAnimation2D>(It->second);
	if (!Anim)
	{
		return false;
	}

	Anim->SetTexture(Texture);

	return true;
}

bool CWorldAssetManager::SetTexture(const std::string& AnimKey, const std::string& TexKey)
{
	auto InnerKey = "Animation2D_" + AnimKey;

	auto It = Assets.find(InnerKey);
	if (It == Assets.end())
	{
		return false;
	}

	if (auto Tex = FindTexture(TexKey).lock())
	{
		std::dynamic_pointer_cast<CAnimation2D>(It->second)->SetTexture(Tex);
	}

	return false;
}

bool CWorldAssetManager::SetTexture(const std::string& AnimKey, const std::string& TexKey, const TCHAR* FileName,
	const std::string& PathName)
{
	auto InnerKey = "Animation2D_" + AnimKey;

	auto It = Assets.find(InnerKey);
	if (It == Assets.end())
	{
		return false;
	}

	if (!LoadTexture(TexKey, FileName, PathName))
	{
		return false;
	}

	auto Anim = std::dynamic_pointer_cast<CAnimation2D>(It->second);
	if (!Anim)
	{
		return false;
	}

	Anim->SetTexture(FindTexture(TexKey));

	return true;
}

bool CWorldAssetManager::SetTextureFullPath(const std::string& AnimKey, const std::string& TexKey,
	const TCHAR* FullPath)
{
	auto InnerKey = "Animation2D_" + AnimKey;

	auto It = Assets.find(InnerKey);
	if (It == Assets.end())
	{
		return false;
	}

	if (!LoadTextureFullPath(TexKey, FullPath))
	{
		return false;
	}

	auto Anim = std::dynamic_pointer_cast<CAnimation2D>(It->second);
	if (!Anim)
	{
		return false;
	}

	Anim->SetTexture(FindTexture(TexKey));

	return true;
}

bool CWorldAssetManager::SetTextures(const std::string& AnimKey, const std::string& TexKey,
	std::vector<const TCHAR*>& FileNames, const std::string& PathName)
{
	auto InnerKey = "Animation2D_" + AnimKey;

	auto It = Assets.find(InnerKey);
	if (It == Assets.end())
	{
		return false;
	}

	if (!LoadTextures(TexKey, FileNames, PathName))
	{
		return false;
	}

	auto Anim = std::dynamic_pointer_cast<CAnimation2D>(It->second);
	if (!Anim)
	{
		return false;
	}

	Anim->SetTexture(FindTexture(TexKey));

	return true;
}

bool CWorldAssetManager::SetTexturesFullPath(const std::string& AnimKey, const std::string& TexKey,
	std::vector<const TCHAR*>& FullPaths)
{
	auto InnerKey = "Animation2D_" + AnimKey;

	auto It = Assets.find(InnerKey);
	if (It == Assets.end())
	{
		return false;
	}

	if (!LoadTexturesFullPath(TexKey, FullPaths))
	{
		return false;
	}

	auto Anim = std::dynamic_pointer_cast<CAnimation2D>(It->second);
	if (!Anim)
	{
		return false;
	}

	Anim->SetTexture(FindTexture(TexKey));

	return true;
}

bool CWorldAssetManager::AddFrame(const std::string& AnimKey, FVector2 Start, FVector2 Size)
{
	auto InnerKey = "Animation2D_" + AnimKey;

	auto It = Assets.find(InnerKey);
	if (It == Assets.end())
	{
		return false;
	}

	auto Anim = std::dynamic_pointer_cast<CAnimation2D>(It->second);
	if (!Anim)
	{
		return false;
	}

	Anim->AddFrame(Start, Size);

	return true;
}

bool CWorldAssetManager::AddFrame(const std::string& AnimKey, float x, float y, float w, float h)
{
	auto InnerKey = "Animation2D_" + AnimKey;

	auto It = Assets.find(InnerKey);
	if (It == Assets.end())
	{
		return false;
	}

	auto Anim = std::dynamic_pointer_cast<CAnimation2D>(It->second);
	if (!Anim)
	{
		return false;
	}

	Anim->AddFrame(x, y, w, h);

	return true;
}

bool CWorldAssetManager::AddFrame(const std::string& AnimKey, int Count, FVector2 Start, FVector2 Size)
{
	auto InnerKey = "Animation2D_" + AnimKey;

	auto It = Assets.find(InnerKey);
	if (It == Assets.end())
	{
		return false;
	}

	auto Anim = std::dynamic_pointer_cast<CAnimation2D>(It->second);
	if (!Anim)
	{
		return false;
	}

	Anim->AddFrame(Count, Start, Size);

	return true;
}

bool CWorldAssetManager::AddFrame(const std::string& AnimKey, int Count, float x, float y, float w, float h)
{
	auto InnerKey = "Animation2D_" + AnimKey;

	auto It = Assets.find(InnerKey);
	if (It == Assets.end())
	{
		return false;
	}

	auto Anim = std::dynamic_pointer_cast<CAnimation2D>(It->second);
	if (!Anim)
	{
		return false;
	}

	Anim->AddFrame(Count, x, y, w, h);

	return true;
}

bool CWorldAssetManager::LoadSound(const std::string& Key, const std::string& GroupKey, bool bLoop,
	const char* FileName, const std::string& PathName)
{
	if (auto SoundMgr = CAssetManager::GetInst()->GetSoundManager().lock())
	{
		std::string InnerKey = "Sound_" + Key;
		if (!SoundMgr->LoadSound(InnerKey, GroupKey, bLoop, FileName, PathName))
		{
			return false;
		}

		auto It = Assets.find(InnerKey);
		if (It == Assets.end())
		{
			Assets.emplace(InnerKey, SoundMgr->FindSound(InnerKey));
		}

		return true;
	}

	return false;
}

std::weak_ptr<CSound> CWorldAssetManager::FindSound(const std::string& Key)
{
	std::string InnerKey = "Sound_" + Key;

	if (auto It = Assets.find(InnerKey); It == Assets.end())
	{
		if (auto SoundMgr = CAssetManager::GetInst()->GetSoundManager().lock())
		{
			if (auto Sound = SoundMgr->FindSound(InnerKey).lock())
			{
				auto [NewIt, Success] = Assets.emplace(InnerKey, Sound);
				return std::dynamic_pointer_cast<CSound>(NewIt->second);
			}
		}

		return {};
	}
	else
	{
		return std::dynamic_pointer_cast<CSound>(It->second);
	}
}

void CWorldAssetManager::SoundPlay(const std::string& Key)
{
	std::string	InnerKey = "Sound_" + Key;

	auto It = Assets.find(InnerKey);
	if (It != Assets.end())
	{
		auto Sound = std::dynamic_pointer_cast<CSound>(It->second);

		Sound->Play();
	}
}

void CWorldAssetManager::SoundStop(const std::string& Key)
{
	std::string	InnerKey = "Sound_" + Key;

	auto It = Assets.find(InnerKey);
	if (It != Assets.end())
	{
		auto Sound = std::dynamic_pointer_cast<CSound>(It->second);

		Sound->Stop();
	}
}

void CWorldAssetManager::SoundPause(const std::string& Key)
{
	std::string	InnerKey = "Sound_" + Key;

	auto It = Assets.find(InnerKey);
	if (It != Assets.end())
	{
		auto Sound = std::dynamic_pointer_cast<CSound>(It->second);

		Sound->Pause();
	}
}

void CWorldAssetManager::SoundResume(const std::string& Key)
{
	std::string	InnerKey = "Sound_" + Key;

	auto It = Assets.find(InnerKey);
	if (It != Assets.end())
	{
		auto Sound = std::dynamic_pointer_cast<CSound>(It->second);

		Sound->Resume();
	}
}

bool CWorldAssetManager::Init()
{
	FVertexColor CenterRectColor[4] =
	{
		FVertexColor(-0.5f, 0.5f, 0.f, 1.f, 0.f, 0.f, 1.f),
		FVertexColor(0.5f, 0.5f, 0.f, 0.f, 1.f, 0.f, 1.f),
		FVertexColor(-0.5f, -0.5f, 0.f, 0.f, 0.f, 1.f, 1.f),
		FVertexColor(0.5f, -0.5f, 0.f, 1.f, 1.f, 0.f, 1.f)
	};

	unsigned short	CenterRectColorIdx[6] = { 0, 1, 3, 0, 3, 2 };

	if (!CreateMesh("CenterRectColor", CenterRectColor, sizeof(FVertexColor),
		4, D3D11_USAGE_IMMUTABLE, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		CenterRectColorIdx, 2, 6, DXGI_FORMAT_R16_UINT,
		D3D11_USAGE_IMMUTABLE))
	{
		return false;
	}

	return true;
}

void CWorldAssetManager::Update(const float DeltaTime)
{
}

void CWorldAssetManager::ClearAsset()
{
	for (const auto& Asset : Assets | std::views::values)
	{
		if (Asset->GetAssetType() == EAssetType::Sound)
		{
			if (auto Sound = std::dynamic_pointer_cast<CSound>(Asset))
			{
				Sound->Stop();
			}
		}
	}
}

CWorldAssetManager::~CWorldAssetManager()
{
	for (auto& Asset : Assets)
	{

	}
}

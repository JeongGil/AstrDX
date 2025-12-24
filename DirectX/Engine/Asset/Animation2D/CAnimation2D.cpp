#include "CAnimation2D.h"

#include "../CAssetManager.h"
#include "../Texture/CTextureManager.h"

void CAnimation2D::SetTexture(const std::weak_ptr<CTexture>& Texture)
{
	this->Texture = Texture;
}

void CAnimation2D::SetTexture(const std::string& Key)
{
	if (auto TexMgr = CAssetManager::GetInst()->GetTextureManager().lock())
	{
		Texture = TexMgr->FindTexture(Key);
	}
}

void CAnimation2D::SetTexture(const std::string& Key, const TCHAR* FileName, const std::string& PathName)
{
	if (auto TexMgr = CAssetManager::GetInst()->GetTextureManager().lock())
	{
		TexMgr->LoadTexture(Key, FileName, PathName);
		Texture = TexMgr->FindTexture(Key);
	}
}

void CAnimation2D::SetTextureFullPath(const std::string& Key, const TCHAR* FullPath)
{
	if (auto TexMgr = CAssetManager::GetInst()->GetTextureManager().lock())
	{
		TexMgr->LoadTextureFullPath(Key, FullPath);
		Texture = TexMgr->FindTexture(Key);
	}
}

void CAnimation2D::SetTextures(const std::string& Key, std::vector<const TCHAR*>& FileNames,
	const std::string& PathName)
{
	if (auto TexMgr = CAssetManager::GetInst()->GetTextureManager().lock())
	{
		TexMgr->LoadTextures(Key, FileNames, PathName);
		Texture = TexMgr->FindTexture(Key);
	}
}

void CAnimation2D::SetTexturesFullPath(const std::string& Key, std::vector<const TCHAR*>& FullPaths)
{
	if (auto TexMgr = CAssetManager::GetInst()->GetTextureManager().lock())
	{
		TexMgr->LoadTexturesFullPath(Key, FullPaths);
		Texture = TexMgr->FindTexture(Key);
	}
}

void CAnimation2D::AddFrame(FVector2 Start, FVector2 Size)
{
	Frames.emplace_back(Start, Size);
}

void CAnimation2D::AddFrame(float x, float y, float w, float h)
{
	Frames.emplace_back(x, y, w, h);
}

void CAnimation2D::AddFrame(int Count, FVector2 Start, FVector2 Size)
{
	for (int i = 0; i < Count; i++)
	{
		Frames.emplace_back(Start, Size);
	}
}

void CAnimation2D::AddFrame(int Count, float x, float y, float w, float h)
{
	for (int i = 0; i < Count; i++)
	{
		Frames.emplace_back(x, y, w, h);
	}
}

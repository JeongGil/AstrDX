#include "CAnimation2DManager.h"

bool CAnimation2DManager::Init()
{
	return true;
}

void CAnimation2DManager::ReleaseAsset(const std::string& Key)
{
	auto It = Animations.find(Key);
	if (It != Animations.end())
	{
		if (It->second.use_count() == 1)
		{
			Animations.erase(It);
		}
	}
}

bool CAnimation2DManager::CreateAnimation(const std::string& AnimKey)
{
	if (Animations.contains(AnimKey))
	{
		return false;
	}

	std::shared_ptr<CAnimation2D> NewAnim(new CAnimation2D);
	NewAnim->SetKey(AnimKey);

	Animations.emplace(AnimKey, NewAnim);

	return true;
}

std::weak_ptr<CAnimation2D> CAnimation2DManager::FindAnimation(const std::string& AnimKey)
{
	auto It =Animations.find(AnimKey);
	return It == Animations.end() ? std::weak_ptr<CAnimation2D>() : It->second;
}

bool CAnimation2DManager::SetAnimation2DTextureType(const std::string& AnimKey, EAnimation2DTextureType TextureType)
{
	auto It = Animations.find(AnimKey);
	if (It == Animations.end())
	{
		return false;
	}

	It->second->SetTextureType(TextureType);

	return true;
}

bool CAnimation2DManager::SetTexture(const std::string& AnimKey, const std::weak_ptr<CTexture>& Texture)
{
	auto It = Animations.find(AnimKey);
	if (It == Animations.end())
	{
		return false;
	}

	It->second->SetTexture(Texture);

	return true;
}

bool CAnimation2DManager::SetTexture(const std::string& AnimKey, const std::string& TexKey)
{
	auto It = Animations.find(AnimKey);
	if (It == Animations.end())
	{
		return false;
	}

	It->second->SetTexture(TexKey);

	return true;
}

bool CAnimation2DManager::SetTexture(const std::string& AnimKey, const std::string& TexKey, const TCHAR* FileName,
                                     const std::string& PathName)
{
	auto It = Animations.find(AnimKey);
	if (It == Animations.end())
	{
		return false;
	}

	It->second->SetTexture(TexKey, FileName, PathName);

	return true;
}

bool CAnimation2DManager::SetTextureFullPath(const std::string& AnimKey, const std::string& TexKey,
                                             const TCHAR* FullPath)
{
	auto It = Animations.find(AnimKey);
	if (It == Animations.end())
	{
		return false;
	}

	It->second->SetTexture(TexKey, FullPath);

	return true;
}

bool CAnimation2DManager::SetTextures(const std::string& AnimKey, const std::string& TexKey,
                                      std::vector<const TCHAR*>& FileNames, const std::string& PathName)
{
	auto It = Animations.find(AnimKey);
	if (It == Animations.end())
	{
		return false;
	}

	It->second->SetTextures(TexKey, FileNames, PathName);

	return true;
}

bool CAnimation2DManager::SetTexturesFullPath(const std::string& AnimKey, const std::string& TexKey,
                                              std::vector<const TCHAR*>& FullPaths)
{
	auto It = Animations.find(AnimKey);
	if (It == Animations.end())
	{
		return false;
	}

	It->second->SetTexturesFullPath(TexKey, FullPaths);

	return true;
}

bool CAnimation2DManager::AddFrame(const std::string& AnimKey, FVector2 Start, FVector2 Size)
{
	auto It = Animations.find(AnimKey);
	if (It == Animations.end())
	{
		return false;
	}

	It->second->AddFrame(Start, Size);

	return true;
}

bool CAnimation2DManager::AddFrame(const std::string& AnimKey, float x, float y, float w, float h)
{
	auto It = Animations.find(AnimKey);
	if (It == Animations.end())
	{
		return false;
	}

	It->second->AddFrame(x, y, w, h);

	return true;
}

bool CAnimation2DManager::AddFrame(const std::string& AnimKey, int Count, FVector2 Start, FVector2 Size)
{
	auto It = Animations.find(AnimKey);
	if (It == Animations.end())
	{
		return false;
	}

	It->second->AddFrame(Count, Start, Size);

	return true;
}

bool CAnimation2DManager::AddFrame(const std::string& AnimKey, int Count, float x, float y, float w, float h)
{
	auto It = Animations.find(AnimKey);
	if (It == Animations.end())
	{
		return false;
	}

	It->second->AddFrame(Count, x, y, w, h);

	return true;
}

#pragma once

#include "CAnimation2D.h"

class CAnimation2DManager
{
	friend class CAssetManager;

private:
	CAnimation2DManager() = default;

public:
	~CAnimation2DManager() = default;

	bool Init();

	bool CreateAnimation(const std::string& AnimKey);
	std::weak_ptr<CAnimation2D> FindAnimation(const std::string& AnimKey);

	bool SetAnimation2DTextureType(const std::string& AnimKey, EAnimation2DTextureType TextureType);

	bool SetTexture(const std::string& AnimKey, const std::weak_ptr<CTexture>& Texture);

	bool SetTexture(const std::string& AnimKey, const std::string& TexKey);
	bool SetTexture(const std::string& AnimKey, const std::string& TexKey, const TCHAR* FileName,
	                const std::string& PathName = "Texture");
	bool SetTextureFullPath(const std::string& AnimKey, const std::string& TexKey, const TCHAR* FullPath);

	bool SetTextures(const std::string& AnimKey, const std::string& TexKey, std::vector<const TCHAR*>& FileNames,
	                 const std::string& PathName = "Texture");
	bool SetTexturesFullPath(const std::string& AnimKey, const std::string& TexKey,
	                         std::vector<const TCHAR*>& FullPaths);

	bool AddFrame(const std::string& AnimKey, FVector2 Start, FVector2 Size);
	bool AddFrame(const std::string& AnimKey, float x, float y, float w, float h);

	bool AddFrame(const std::string& AnimKey, int Count, FVector2 Start, FVector2 Size);
	bool AddFrame(const std::string& AnimKey, int Count, float x, float y, float w, float h);

private:
	std::unordered_map < std::string, std::shared_ptr<CAnimation2D>> Animations;
};


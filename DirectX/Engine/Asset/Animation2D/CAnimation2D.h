#pragma once
#include "../../CObject.h"

class CTexture;

class CAnimation2D :
    public CObject
{
	friend class CAnimation2DManager;

public:
	[[nodiscard]] const std::string& GetKey() const
	{
		return Key;
	}

	void SetKey(const std::string& Key)
	{
		this->Key = Key;
	}

	[[nodiscard]] EAnimation2DTextureType GetTextureType() const
	{
		return TextureType;
	}

	void SetTextureType(EAnimation2DTextureType TextureType)
	{
		this->TextureType = TextureType;
	}

	[[nodiscard]] const std::weak_ptr<CTexture>& GetTexture() const
	{
		return Texture;
	}

	[[nodiscard]] const FTextureFrame& GetFrame(int Index) const
	{
		return Frames[Index];
	}

	[[nodiscard]] int GetFrameCount() const
	{
		return static_cast<int>(Frames.size());
	}

	void SetTexture(const std::weak_ptr<CTexture>& Texture);

	void SetTexture(const std::string& Key);
	void SetTexture(const std::string& Key, const TCHAR* FileName, const std::string& PathName = "Texture");
	void SetTextureFullPath(const std::string& Key, const TCHAR* FullPath);

	void SetTextures(const std::string& Key, std::vector<const TCHAR*>& FileNames, const std::string& PathName = "Texture");
	void SetTexturesFullPath(const std::string& Key, std::vector<const TCHAR*>& FullPaths);

	void AddFrame(FVector2 Start, FVector2 Size);
	void AddFrame(float x, float y, float w, float h);

	void AddFrame(int Count, FVector2 Start, FVector2 Size);
	void AddFrame(int Count, float x, float y, float w, float h);

protected:
	std::string Key;
	std::weak_ptr<CTexture> Texture;
	EAnimation2DTextureType TextureType = EAnimation2DTextureType::SpriteSheet;

	std::vector<FTextureFrame> Frames;
};


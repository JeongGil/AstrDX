#pragma once
#include "../CAsset.h"

#include "DirectXTex.h"

#ifdef _DEBUG

#pragma comment(lib, "DirectXTex_Debug.lib")

#else

#pragma comment(lib, "DirectXTex.lib")

#endif

enum class ETextureType
{
	Normal,
	Frame,
	Array,
};

struct FTextureInfo
{
	// An object for storing the pixel information of a texture.
	DirectX::ScratchImage* Image = nullptr;

	// An object for sending texture information to the shader to render the screen using the above pixel information.
	ID3D11ShaderResourceView* SRV = nullptr;

	unsigned int Width = 0;
	unsigned int Height = 0;

	~FTextureInfo()
	{
		SAFE_RELEASE(Image);
		SAFE_RELEASE(SRV);
	}
};

class CTexture :
	public CAsset
{
	friend class CTextureManager;

protected:
	CTexture();

public:
	~CTexture() override;

	bool LoadTexture(const TCHAR* FileName, const std::string& PathName);
	bool LoadTextureFullPath(const TCHAR* FullPath);
	bool LoadTextures(const std::vector<const TCHAR*>& FileNames, const std::string& PathName);
	bool LoadTexturesFullPath(const std::vector<const TCHAR*>& FullPaths);
	bool LoadTextureArray(const std::vector<const TCHAR*>& FileNames, const std::string& PathName);
	bool LoadTextureArrayFullPath(const std::vector<const TCHAR*>& FullPaths);

	void SetShader(UINT Register, int ShaderBufferType, int TextureIndex);

	const FTextureInfo* GetTexture(size_t Index = 0) const
	{
		return Textures[Index];
	}

	size_t GetTextureCount() const
	{
		return Textures.size();
	}

protected:
	// Designed to allow multiple images to be composed into a single texture.
	std::vector<FTextureInfo*> Textures;
	ETextureType TextureType{ ETextureType::Normal };
	ID3D11ShaderResourceView* ArraySRV{};
	int ArrayCount{ 0 };

private:
	bool CreateResourceView(size_t Index = 0);
	bool CreateResourceViewArray();
};

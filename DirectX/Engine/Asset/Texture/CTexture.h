#pragma once
#include "../../CObject.h"
#include "DirectXTex.h"
#include "../Shader/CBufferContainer.h"

#ifdef _DEBUG

#pragma comment(lib, "DirectXTex_Debug.lib")

#else

#pragma comment(lib, "DirectXTex.lib")

#endif

struct FTextureInfo;

class CTexture :
	public CObject
{
	friend class CTextureManager;

protected:
	CTexture() = default;

public:
	~CTexture() override;

	bool LoadTexture(const TCHAR* FileName, const std::string& PathName);
	bool LoadTextureFullPath(const TCHAR* FullPath);
	bool LoadTextures(const std::vector<const TCHAR*>& FileNames, const std::string& PathName);
	bool LoadTexturesFullPath(const std::vector<const TCHAR*>& FullPaths);

	void SetShader(UINT Register, int ShaderBufferType, int TextureIndex);

	const std::string& GetName() const
	{
		return Name;
	}

	const FTextureInfo* GetTexture(size_t Index = 0) const
	{
		return TextureInfos[Index];
	}

	size_t GetTextureCount() const
	{
		return TextureInfos.size();
	}

protected:
	std::string Name;
	// Designed to allow multiple images to be composed into a single texture.
	std::vector<FTextureInfo*> TextureInfos;

private:
	bool CreateResourceView(size_t Index = 0);
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

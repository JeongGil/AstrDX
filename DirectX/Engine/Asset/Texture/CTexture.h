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
protected:
	CTexture() = default;

public:
	~CTexture() override;

	bool LoadTexture(const TCHAR* FileName, const std::string& PathName);
	bool LoadTexture(const TCHAR* FullPath);
	bool LoadTexture(const std::vector<const TCHAR*>& FileNames, const std::string& PathName);
	bool LoadTexture(const std::vector<const TCHAR*>& FullPaths);

	void SetShader(UINT Register, EShaderBufferType::Type ShaderBufferType, size_t TextureIndex);

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

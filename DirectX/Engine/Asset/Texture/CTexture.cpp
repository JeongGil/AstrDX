#include "CTexture.h"

#include "CTextureManager.h"
#include "../CPathManager.h"
#include "../../CDevice.h"
#include "../Shader/CBufferContainer.h"

CTexture::CTexture()
{
	AssetType = EAssetType::Texture;
}

CTexture::~CTexture()
{
	SAFE_RELEASE(ArraySRV);

	for (auto& Texture : Textures)
	{
		SAFE_DELETE(Texture);
	}
}

bool CTexture::LoadTexture(const TCHAR* FileName, const std::string& PathName)
{
	const TCHAR* BasePath = CPathManager::FindPath(PathName);

	TCHAR FullPath[MAX_PATH] = {};

	lstrcpy(FullPath, BasePath);
	lstrcat(FullPath, FileName);

	return LoadTextureFullPath(FullPath);
}

bool CTexture::LoadTextureFullPath(const TCHAR* FullPath)
{
	auto Texture = new FTextureInfo;

	// The loading function differs for each file extension.
	// 1. .dds 2. .tga 3. Other file extensions
	// Retrieve the file extension from the full path.
	TCHAR FileExt[_MAX_EXT] = {};

	// Retrieve the file extension.
	// The _wsplitpath_s function is used to split a path into its components.
	_wsplitpath_s(FullPath, nullptr, 0, nullptr, 0, nullptr, 0,
		FileExt, _MAX_EXT);

	// Convert the Unicode string file extension obtained above into a multibyte string.
	char ConvertExt[_MAX_EXT] = {};

	// Get the length required to convert to a multibyte string.
	int Count = WideCharToMultiByte(CP_ACP, 0, FileExt, -1, nullptr, 0, nullptr, nullptr);
	WideCharToMultiByte(CP_ACP, 0, FileExt, -1, ConvertExt, Count, nullptr, nullptr);

	_strupr_s(ConvertExt);

	auto Image = new DirectX::ScratchImage;

	if (strcmp(ConvertExt, ".DDS") == 0)
	{
		if (FAILED(DirectX::LoadFromDDSFile(FullPath, DirectX::DDS_FLAGS_NONE, nullptr, *Image)))
		{
			SAFE_DELETE(Texture);
			SAFE_DELETE(Image);

			return false;
		}
	}
	else if (strcmp(ConvertExt, ".TGA") == 0)
	{
		if (FAILED(DirectX::LoadFromTGAFile(FullPath, nullptr, *Image)))
		{
			SAFE_DELETE(Texture);
			SAFE_DELETE(Image);

			return false;
		}
	}
	else
	{
		if (FAILED(DirectX::LoadFromWICFile(FullPath, DirectX::WIC_FLAGS_NONE, nullptr, *Image)))
		{
			SAFE_DELETE(Texture);
			SAFE_DELETE(Image);

			return false;
		}
	}

	Texture->Image = Image;
	Texture->FullPath = FullPath;

	Textures.push_back(Texture);

	// Create a ShaderResourceView for rendering using the loaded image information.
	return CreateResourceView(Textures.size() - 1);
}

bool CTexture::LoadTextures(const std::vector<const TCHAR*>& FileNames, const std::string& PathName)
{
	TextureType = ETextureType::Frame;

	for (const auto& FileName : FileNames)
	{
		if (!LoadTexture(FileName, PathName))
		{
			return false;
		}
	}

	return true;
}

bool CTexture::LoadTexturesFullPath(const std::vector<const TCHAR*>& FullPaths)
{
	TextureType = ETextureType::Frame;

	for (const auto& FullPath : FullPaths)
	{
		if (!LoadTextureFullPath(FullPath))
		{
			return false;
		}
	}

	return true;
}

bool CTexture::LoadTextureArray(const std::vector<const TCHAR*>& FileNames, const std::string& PathName)
{
	auto BasePath = CPathManager::FindPath(PathName);

	std::vector<const TCHAR*> FullPaths;

	for (const auto& FileName : FileNames)
	{
		TCHAR* FullPath = new TCHAR[MAX_PATH];

		lstrcpy(FullPath, BasePath);
		lstrcat(FullPath, FileName);

		FullPaths.push_back(FullPath);
	}

	bool Result = LoadTextureArrayFullPath(FullPaths);

	for (auto& FullPath : FullPaths)
	{
		if (FullPath)
		{
			delete[] FullPath;
		}
	}

	return Result;
}

bool CTexture::LoadTextureArrayFullPath(const std::vector<const TCHAR*>& FullPaths)
{
	TextureType = ETextureType::Array;

	size_t Size = FullPaths.size();
	ArrayCount = static_cast<int>(Size);

	for (const auto& FullPath : FullPaths)
	{
		FTextureInfo* Texture = new FTextureInfo;

		TCHAR FileExt[_MAX_EXT] = {};

		_wsplitpath_s(FullPath, nullptr, 0, nullptr, 0,
			nullptr, 0, FileExt, _MAX_EXT);

		char ConvertExt[_MAX_EXT] = {};
		int	Count = WideCharToMultiByte(CP_ACP, 0, FileExt, -1,
			nullptr, 0, nullptr, nullptr);
		WideCharToMultiByte(CP_ACP, 0, FileExt, -1,
			ConvertExt, Count, nullptr, nullptr);

		_strupr_s(ConvertExt);

		DirectX::ScratchImage* Image = new DirectX::ScratchImage;

		if (strcmp(ConvertExt, ".DDS") == 0)
		{
			if (FAILED(DirectX::LoadFromDDSFile(FullPath, DirectX::DDS_FLAGS_NONE, nullptr, *Image)))
			{
				SAFE_DELETE(Texture);
				SAFE_DELETE(Image);
				return false;
			}
		}
		else if (strcmp(ConvertExt, ".TGA") == 0)
		{
			if (FAILED(DirectX::LoadFromTGAFile(FullPath, nullptr, *Image)))
			{
				SAFE_DELETE(Texture);
				SAFE_DELETE(Image);
				return false;
			}
		}
		else
		{
			if (FAILED(DirectX::LoadFromWICFile(FullPath, DirectX::WIC_FLAGS_NONE, nullptr, *Image)))
			{
				SAFE_DELETE(Texture);
				SAFE_DELETE(Image);
				return false;
			}
		}

		Texture->Image = Image;
		Texture->FullPath = FullPath;

		Texture->Width = (unsigned int)Image->GetImages()[0].width;
		Texture->Height = (unsigned int)Image->GetImages()[0].height;

		Textures.push_back(Texture);
	}

	return CreateResourceViewArray();
}

bool CTexture::CreateResourceViewArray()
{
	auto ImageArray = new DirectX::ScratchImage;

	size_t MipLevel = Textures[0]->Image->GetMetadata().mipLevels;
	size_t Count = Textures.size();

	if (FAILED(ImageArray->Initialize2D(
		Textures[0]->Image->GetMetadata().format,
		(size_t)Textures[0]->Width,
		(size_t)Textures[0]->Height,
		Count, MipLevel)))
	{
		return false;
	}

	for (size_t i = 0; i < Count; ++i)
	{
		const DirectX::Image* Images = Textures[i]->Image->GetImages();

		MipLevel = Textures[i]->Image->GetMetadata().mipLevels;

		for (size_t j = 0; j < MipLevel; ++j)
		{
			const DirectX::Image* Src = &ImageArray->GetImages()[i * MipLevel + j];
			const DirectX::Image* Dest = &Images[j];

			// 픽셀 복사
			memcpy(Src->pixels, Dest->pixels, Src->slicePitch);
		}

		CreateResourceView((int)i);
	}

	ID3D11Device* Device = CDevice::GetInst()->GetDevice();

	ID3D11Texture2D* Texture = nullptr;

	if (FAILED(DirectX::CreateTextureEx(
		Device,
		ImageArray->GetImages(),
		ImageArray->GetImageCount(),
		ImageArray->GetMetadata(),
		D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE,
		0, 0, DirectX::CREATETEX_FLAGS::CREATETEX_DEFAULT,
		(ID3D11Resource**)&Texture)))
	{
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC	Desc{};
	Desc.Format = Textures[0]->Image->GetMetadata().format;
	Desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	Desc.Texture2DArray.MostDetailedMip = 0;
	Desc.Texture2DArray.MipLevels = Textures[0]->Image->GetMetadata().mipLevels;
	Desc.Texture2DArray.FirstArraySlice = 0;
	Desc.Texture2DArray.ArraySize = (UINT)Count;

	if (FAILED(Device->CreateShaderResourceView(Texture, &Desc, &ArraySRV)))
	{
		return false;
	}

	SAFE_DELETE(ImageArray);

	SAFE_RELEASE(Texture);

	return true;
}

void CTexture::SetShader(UINT Register, int ShaderBufferType, int TextureIndex)
{
	ID3D11ShaderResourceView* SRV;
	if (TextureType == ETextureType::Array)
	{
		SRV = ArraySRV;
	}
	else
	{
		SRV = Textures[TextureIndex]->SRV;
	}

	if ((ShaderBufferType & EShaderBufferType::Vertex) != 0)
	{
		CDevice::GetInst()->GetContext()->VSSetShaderResources(Register, 1, &SRV);
	}

	if ((ShaderBufferType & EShaderBufferType::Pixel) != 0)
	{
		CDevice::GetInst()->GetContext()->PSSetShaderResources(Register, 1, &SRV);
	}

	if ((ShaderBufferType & EShaderBufferType::Domain) != 0)
	{
		CDevice::GetInst()->GetContext()->DSSetShaderResources(Register, 1, &SRV);
	}

	if ((ShaderBufferType & EShaderBufferType::Hull) != 0)
	{
		CDevice::GetInst()->GetContext()->HSSetShaderResources(Register, 1, &SRV);
	}

	if ((ShaderBufferType & EShaderBufferType::Geometry) != 0)
	{
		CDevice::GetInst()->GetContext()->GSSetShaderResources(Register, 1, &SRV);
	}

	if ((ShaderBufferType & EShaderBufferType::Compute) != 0)
	{
		CDevice::GetInst()->GetContext()->CSSetShaderResources(Register, 1, &SRV);
	}
}

void CTexture::Save(FILE* File)
{
	size_t Size = Textures.size();

	fwrite(&Size, sizeof(size_t), 1, File);

	for (const auto& Texture : Textures)
	{
		size_t Count = Texture->FullPath.length();

		fwrite(&Count, sizeof(size_t), 1, File);
		fwrite(Texture->FullPath.c_str(), sizeof(wchar_t), Count, File);
	}
}

bool CTexture::CreateResourceView(size_t Index)
{
	FTextureInfo*& Texture = Textures[Index];
	DirectX::ScratchImage*& Image = Texture->Image;

	if (FAILED(DirectX::CreateShaderResourceView(
		CDevice::GetInst()->GetDevice(),
		Image->GetImages(),
		Image->GetImageCount(),
		Image->GetMetadata(),
		&Texture->SRV)))
	{
		return false;
	}

	Texture->Width = static_cast<unsigned int>(Image->GetImages()[0].width);
	Texture->Height = static_cast<unsigned int>(Image->GetImages()[0].height);

	return true;
}

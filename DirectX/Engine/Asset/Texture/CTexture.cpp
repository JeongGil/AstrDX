#include "CTexture.h"

#include "../CPathManager.h"
#include "../../CDevice.h"
#include "../Shader/CBufferContainer.h"

CTexture::~CTexture()
{
	for (auto& Texture : TextureInfos)
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

	TextureInfos.push_back(Texture);

	// Create a ShaderResourceView for rendering using the loaded image information.
	return CreateResourceView(TextureInfos.size() - 1);
}

bool CTexture::LoadTextures(const std::vector<const TCHAR*>& FileNames, const std::string& PathName)
{
	return false;
}

bool CTexture::LoadTexturesFullPath(const std::vector<const TCHAR*>& FullPaths)
{
	return false;
}

void CTexture::SetShader(UINT Register, int ShaderBufferType, int TextureIndex)
{
	if ((ShaderBufferType & EShaderBufferType::Vertex) != 0)
	{
		CDevice::GetInst()->GetContext()->VSSetShaderResources(Register, 1, &TextureInfos[TextureIndex]->SRV);
	}

	if ((ShaderBufferType & EShaderBufferType::Pixel) != 0)
	{
		CDevice::GetInst()->GetContext()->PSSetShaderResources(Register, 1, &TextureInfos[TextureIndex]->SRV);
	}

	if ((ShaderBufferType & EShaderBufferType::Domain) != 0)
	{
		CDevice::GetInst()->GetContext()->DSSetShaderResources(Register, 1, &TextureInfos[TextureIndex]->SRV);
	}

	if ((ShaderBufferType & EShaderBufferType::Hull) != 0)
	{
		CDevice::GetInst()->GetContext()->HSSetShaderResources(Register, 1, &TextureInfos[TextureIndex]->SRV);
	}

	if ((ShaderBufferType & EShaderBufferType::Geometry) != 0)
	{
		CDevice::GetInst()->GetContext()->GSSetShaderResources(Register, 1, &TextureInfos[TextureIndex]->SRV);
	}

	if ((ShaderBufferType & EShaderBufferType::Compute) != 0)
	{
		CDevice::GetInst()->GetContext()->CSSetShaderResources(Register, 1, &TextureInfos[TextureIndex]->SRV);
	}
}

bool CTexture::CreateResourceView(size_t Index)
{
	FTextureInfo*& Texture = TextureInfos[Index];
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

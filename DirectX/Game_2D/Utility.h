#pragma once
#include <EngineInfo.h>
#include <Component/CMeshComponent.h>
#include <Component/CColliderBox2D.h>
#include <Asset/Material/CMaterial.h>
#include <Asset/Texture/CTexture.h>

inline std::string TCharToStdString(const TCHAR* tcharStr)
{
	if (!tcharStr) return "";

#ifdef UNICODE
	int size = WideCharToMultiByte(CP_UTF8, 0, tcharStr, -1, nullptr, 0, nullptr, nullptr);
	if (size <= 0) return "";

	std::string result(size - 1, 0);
	WideCharToMultiByte(CP_UTF8, 0, tcharStr, -1, &result[0], size, nullptr, nullptr);
	return result;
#else
	return std::string(tcharStr);
#endif
}

inline void SetMeshAndColliderSizeFromTexture(
	const std::weak_ptr<FMaterialTextureInfo>& MatTexInfo,
	const std::weak_ptr<CMeshComponent>& Mesh,
	const std::weak_ptr<CColliderBox2D>& Collider)
{
	if (auto MatTexInfoPtr = MatTexInfo.lock())
	{
		if (auto Texture = MatTexInfoPtr->Texture.lock())
		{
			auto TexInfo = Texture->GetTexture();
			if (auto MeshPtr = Mesh.lock())
			{
				MeshPtr->SetWorldScale(TexInfo->Width, TexInfo->Height);
			}
			if (auto ColliderPtr = Collider.lock())
			{
				ColliderPtr->SetBoxExtent(TexInfo->Width, TexInfo->Height);
			}
		}
	}
}

#pragma once

#include "../EngineInfo.h"
#include "../Asset/Animation2D/CAnimation2D.h"
#include "../Asset/Mesh/CMesh.h"
#include "../Asset/Shader/CConstantBuffer.h"
#include "../Asset/Sound/CSound.h"
#include "../Asset/Sound/CSoundManager.h"
#include "../Asset/Texture/CTexture.h"

class CWorldAssetManager
{
	friend class CWorld;

public:
#pragma region Mesh

	bool CreateMesh(const std::string& Key, void* Vertices, int VertexSize, int VertexCount,
		D3D11_USAGE VertexUsage, D3D11_PRIMITIVE_TOPOLOGY Topology, void* Indices = nullptr,
		int IndexSize = 0, int IndexCount = 0, DXGI_FORMAT Format = DXGI_FORMAT_UNKNOWN,
		D3D11_USAGE IndexUsage = D3D11_USAGE_DEFAULT);

	std::weak_ptr<CMesh> FindMesh(const std::string& Key);

#pragma endregion

#pragma region Material
#pragma endregion

#pragma region Texture

	bool LoadTexture(const std::string& Key, const TCHAR* FileName, const std::string& PathName);
	bool LoadTextureFullPath(const std::string& Key, const TCHAR* FullPath);
	bool LoadTextures(const std::string& Key, const std::vector<const TCHAR*>& FileNames, const std::string& PathName);
	bool LoadTexturesFullPath(const std::string& Key, const std::vector<const TCHAR*>& FullPaths);
	bool LoadTextureArray(const std::string& Key, const std::vector<const TCHAR*>& FileNames, const std::string& PathName);
	bool LoadTextureArrayFullPath(const std::string& Key, const std::vector<const TCHAR*>& FullPaths);

	std::weak_ptr<CTexture> FindTexture(const std::string& Key);

#pragma endregion

#pragma region Animation2D

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

#pragma endregion

#pragma region Sound

	bool LoadSound(const std::string& Key, const std::string& GroupKey, bool bLoop,
		const char* FileName, const std::string& PathName = "Sound");
	std::weak_ptr<CSound> FindSound(const std::string& Key);

	void SoundPlay(const std::string& Key);
	void SoundStop(const std::string& Key);
	void SoundPause(const std::string& Key);
	void SoundResume(const std::string& Key);

#pragma endregion Sound

	bool Init();
	void Update(const float DeltaTime);

	void ClearAsset();

private:
	std::unordered_map<std::string, std::shared_ptr<CAsset>> Assets;

private:
	CWorldAssetManager() = default;

public:
	~CWorldAssetManager();
};


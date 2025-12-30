#pragma once

#include "../EngineInfo.h"

class CMeshManager;
class CShaderManager;
class CTextureManager;
class CAnimation2DManager;

class CAssetManager
{
public:
	/**
	 * @brief Initializes the asset manager and its associated components.
	 *
	 * This method initializes the `CAssetManager` by first ensuring that the `CPathManager`
	 * is successfully initialized. It then creates and initializes instances of `CMeshManager`
	 * and `CShaderManager`. If any of these components fail to initialize, the method returns `false`.
	 *
	 * @return true if all components are successfully initialized, false otherwise.
	 */
	bool Init();

	void ReleaseAsset(const std::string& Key, EAssetType AssetType);

	[[nodiscard]] std::weak_ptr<CMeshManager> GetMeshManager() const { return MeshManager; }

	[[nodiscard]] std::weak_ptr<CShaderManager> GetShaderManager() const { return ShaderManager; }

	[[nodiscard]] std::weak_ptr<CTextureManager> GetTextureManager() const { return TextureManager; }

	[[nodiscard]] std::weak_ptr<CAnimation2DManager> GetAnimation2DManager() const { return Animation2DManager; }

private:
	std::shared_ptr<CMeshManager> MeshManager;
	std::shared_ptr<CShaderManager> ShaderManager;
	std::shared_ptr<CTextureManager> TextureManager;
	std::shared_ptr<CAnimation2DManager> Animation2DManager;

public:
	static CAssetManager* GetInst()
	{
		if (inst == nullptr)
		{
			inst = new CAssetManager;
		}

		return inst;
	}

	static void DestroyInst()
	{
		SAFE_DELETE(inst)
	}

private:
	CAssetManager();
	~CAssetManager();

	inline static CAssetManager* inst = nullptr;
};


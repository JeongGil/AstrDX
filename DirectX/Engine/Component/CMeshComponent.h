#pragma once
#include "CSceneComponent.h"

#include "../Asset/Shader/CBufferContainer.h"
#include "../Asset/Shader/CCBufferTransform.h"

class CCBufferAnimation2D;
class CAnimation2DComponent;
class CShader;
class CMesh;
class CCBufferTransform;
class CMaterial;
class CTexture;
class CRenderState;
struct FMaterialTextureInfo;

class CMeshComponent :
	public CSceneComponent
{
	friend class CGameObject;
	friend CObject;

public:
	void SetMesh(const std::weak_ptr<CMesh>& Mesh);
	void SetMesh(const std::string& Key);

	void SetShader(const std::weak_ptr<CShader>& Shader)
	{
		this->Shader = Shader;
	}

	void SetShader(const std::string& Key);

	void SetBlendState(int SlotIndex, const std::string& Key);
	void SetBlendState(int SlotIndex, const std::weak_ptr<CRenderState>& State);

	void SetMaterialBaseColor(int SlotIndex, float r, float g, float b, float a);
	void SetMaterialBaseColor(int SlotIndex, int r, int g, int b, int a);
	void SetMaterialBaseColor(int SlotIndex, const FVector4& Color);
	void SetMaterialOpacity(int SlotIndex, float Opacity);

	std::weak_ptr<FMaterialTextureInfo> AddTexture(int SlotIdx, const std::weak_ptr<CTexture>& Texture, int Register,
	                                               int ShaderBufferType = EShaderBufferType::Pixel, int Index = 0);

	std::weak_ptr<FMaterialTextureInfo> AddTexture(int SlotIdx, const std::string& Key, int Register,
	                                               int ShaderBufferType = EShaderBufferType::Pixel, int Index = 0);
	std::weak_ptr<FMaterialTextureInfo> AddTexture(int SlotIdx, const std::string& Key, const TCHAR* FileName,
	                                               const std::string& PathName = "Texture", int Register = 0,
	                                               int ShaderBufferType = EShaderBufferType::Pixel, int Index = 0);
	std::weak_ptr<FMaterialTextureInfo> AddTextureFullPath(int SlotIdx, const std::string& Key, const TCHAR* FullPath,
	                                                       int Register = 0,
	                                                       int ShaderBufferType = EShaderBufferType::Pixel,
	                                                       int Index = 0);

	std::weak_ptr<FMaterialTextureInfo> AddTextures(int SlotIdx, const std::string& Key,
	                                                std::vector<const TCHAR*>& FileNames,
	                                                const std::string& PathName = "Texture", int Register = 0,
	                                                int ShaderBufferType = EShaderBufferType::Pixel, int Index = 0);
	std::weak_ptr<FMaterialTextureInfo> AddTexturesFullPath(int SlotIdx, const std::string& Key,
	                                                        std::vector<const TCHAR*>& FullPaths, int Register = 0,
	                                                        int ShaderBufferType = EShaderBufferType::Pixel,
	                                                        int Index = 0);

	bool SetTexture(int SlotIndex, int TextureIndex, const std::weak_ptr<CTexture>& Texture);
	bool SetTextureIndex(int SlotIndex, int TextureIndex);

	void SetAnimationComponent(const std::weak_ptr<CAnimation2DComponent>& AnimComponent)
	{
		this->AnimationComponent = AnimComponent;
	}

	static void CreateEmptyAnimationCBuffer();
	static void ClearEmptyAnimationCBuffer();

protected:
	std::weak_ptr<CShader> Shader;
	std::weak_ptr<CMesh> Mesh;
	std::vector<std::shared_ptr<CMaterial>> MaterialSlot;
	std::shared_ptr<CCBufferTransform> CBufferTransform;

	std::weak_ptr<CAnimation2DComponent> AnimationComponent;
	static std::shared_ptr<CCBufferAnimation2D> EmptyAnimationCBuffer;

public:
	/**
	 * @brief Initializes the mesh component and its associated resources.
	 *
	 * This method sets up the necessary resources for the mesh component, including
	 * the creation and initialization of the constant buffer for transformation matrices.
	 *
	 * @return `true` if the initialization is successful, otherwise `false`.
	 */
	bool Init() override;

	/**
	 * @brief Renders the mesh component using its associated shader and transformation matrices.
	 *
	 * This method performs the rendering of the mesh component by:
	 * - Validating the availability of the associated shader and mesh.
	 * - Setting up the view and projection matrices for the camera.
	 * - Updating the constant buffer with the world, view, and projection matrices.
	 * - Activating the shader and rendering the mesh.
	 * - Invoking the render method of the base class to ensure hierarchical rendering.
	 *
	 * The method ensures that the component is rendered correctly in the 3D scene
	 * with the appropriate transformations and shader effects.
	 */
	void Render() override;

protected:
	CMeshComponent* Clone() const override;

protected:
	CMeshComponent();
	CMeshComponent(const CMeshComponent& other);
	CMeshComponent(CMeshComponent&& other) noexcept;

public:
	~CMeshComponent() override = default;
};


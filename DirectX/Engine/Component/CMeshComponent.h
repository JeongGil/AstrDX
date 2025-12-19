#pragma once
#include "CSceneComponent.h"
#include "../Asset/CAssetManager.h"
#include "../Asset/Mesh/CMeshManager.h"
#include "../Asset/Shader/CCBufferTransform.h"
#include "../Asset/Shader/CShaderManager.h"

class CShader;
class CMesh;
class CCBufferTransform;

class CMeshComponent :
    public CSceneComponent
{
	friend class CGameObject;
	friend CObject;

public:
	void SetMesh(const std::weak_ptr<CMesh>& Mesh)
	{
		this->Mesh = Mesh;
	}

	void SetMesh(const std::string& Key);

	void SetShader(const std::weak_ptr<CShader>& Shader)
	{
		this->Shader = Shader;
	}

	void SetShader(const std::string& Key);

protected:
	std::weak_ptr<CShader> Shader;
	std::weak_ptr<CMesh> Mesh;
	std::shared_ptr<CCBufferTransform> CBufferTransform;

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
	CMeshComponent() = default;

	CMeshComponent(const CMeshComponent& other)
		: CSceneComponent(other),
		  Shader(other.Shader),
		  Mesh(other.Mesh),
		  CBufferTransform(other.CBufferTransform->Clone())
	{
	}

	CMeshComponent(CMeshComponent&& other) noexcept
		: CSceneComponent(std::move(other)),
		  Shader(std::move(other.Shader)),
		  Mesh(std::move(other.Mesh)),
		  CBufferTransform(std::move(other.CBufferTransform))
	{
		other.Shader.reset();
		other.Mesh.reset();
		other.CBufferTransform.reset();
	}

public:
	~CMeshComponent() override = default;
};


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
	bool Init() override;
	void Render() override;
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
		other.CBufferTransform.reset();
	}

public:
	~CMeshComponent() override = default;
};


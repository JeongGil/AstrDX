#include "CMeshComponent.h"

#include "../Asset/Mesh/CMesh.h"
#include "../Asset/Shader/CCBufferTransform.h"
#include "../World/CWorld.h"
#include "../World/CCameraManager.h"

void CMeshComponent::SetMesh(const std::string& Key)
{
	auto WeakManager = CAssetManager::GetInst()->GetMeshManager();
	if (auto MeshManager = WeakManager.lock())
	{
		Mesh = MeshManager->FindMesh(Key);
	}
}

void CMeshComponent::SetShader(const std::string& Key)
{
	auto WeakManager = CAssetManager::GetInst()->GetShaderManager();
	if (auto ShaderManager = WeakManager.lock())
	{
		Shader = ShaderManager->FindShader(Key);
	}
}

bool CMeshComponent::Init()
{
	CBufferTransform.reset(new CCBufferTransform);
	if (!CBufferTransform->Init())
	{
		return false;
	}

	return true;
}

void CMeshComponent::Render()
{
	if (Shader.expired() || Mesh.expired())
	{
		return;
	}

	auto Shader = this->Shader.lock();
	auto Mesh = this->Mesh.lock();
	
	FMatrix ViewMat;
	FMatrix ProjMat;

	if (auto World=this->World.lock())
	{
		if (auto CamMgr = World->GetCameraManager().lock())
		{
			ViewMat = CamMgr->GetViewMatrix();
			ProjMat = CamMgr->GetProjectionMatrix();
		}
	}

	CBufferTransform->SetWorldMatrix(WorldMatrix);
	CBufferTransform->SetViewMatrix(ViewMat);
	CBufferTransform->SetProjMatrix(ProjMat);

	CBufferTransform->UpdateBuffer();

	Shader->SetShader();

	Mesh->Render();

	CSceneComponent::Render();
}

CMeshComponent* CMeshComponent::Clone() const
{
	return new CMeshComponent(*this);
}

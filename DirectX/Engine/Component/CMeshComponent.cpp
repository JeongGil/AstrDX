#include "CMeshComponent.h"

#include "../Asset/Mesh/CMesh.h"
#include "../Asset/Shader/CCBufferTransform.h"

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
	CBufferTransform->Init();

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

	FVector CamPos(0, 0, -5);
	FVector LookAt(0, 0, 0);
	FVector CamUp(0, 1, 0);

	FMatrix ViewMat = DirectX::XMMatrixLookAtLH(CamPos.Convert(), LookAt.Convert(), CamUp.Convert());

	float Radian = DirectX::XMConvertToRadians(60);
	FMatrix ProjMat = DirectX::XMMatrixPerspectiveFovLH(Radian, 1280.f/720, 0.1f, 1000);

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

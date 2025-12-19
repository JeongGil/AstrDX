#include "CMeshComponent.h"

#include "../Asset/Material/CMaterial.h"
#include "../Asset/Mesh/CMesh.h"
#include "../Asset/Shader/CCBufferTransform.h"
#include "../World/CWorld.h"
#include "../World/CCameraManager.h"

void CMeshComponent::SetMesh(const std::weak_ptr<CMesh>& Mesh)
{
	this->Mesh = Mesh;

	if (auto Mesh = this->Mesh.lock())
	{
		Materials.clear();
		auto MeshSlotCount = Mesh->GetSlotCount();
		for (size_t i = 0; i < MeshSlotCount; ++i)
		{
			auto Slot = Mesh->GetSlot(i);
			Materials.emplace_back(std::shared_ptr<CMaterial>(Slot->Material->Clone()));
		}
	}
}

void CMeshComponent::SetMesh(const std::string& Key)
{
	auto WeakManager = CAssetManager::GetInst()->GetMeshManager();
	if (auto MeshManager = WeakManager.lock())
	{
		SetMesh(MeshManager->FindMesh(Key));
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

void CMeshComponent::SetMaterialBaseColor(int SlotIndex, float r, float g, float b, float a)
{
	Materials[SlotIndex]->SetBaseColor(r, g, b, a);
}

void CMeshComponent::SetMaterialBaseColor(int SlotIndex, int r, int g, int b, int a)
{
	Materials[SlotIndex]->SetBaseColor(r, g, b, a);
}

void CMeshComponent::SetMaterialBaseColor(int SlotIndex, const FVector4& Color)
{
	Materials[SlotIndex]->SetBaseColor(Color);
}

void CMeshComponent::SetMaterialOpacity(int SlotIndex, float Opacity)
{
	Materials[SlotIndex]->SetOpacity(Opacity);
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

	if (auto World = this->World.lock())
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

	auto MeshSlotCount = Materials.size();
	for (size_t i = 0; i < MeshSlotCount; i++)
	{
		if (Materials[i])
		{
			Materials[i]->UpdateConstantBuffer();
		}

		Mesh->Render(i);
	}

	CSceneComponent::Render();
}

CMeshComponent* CMeshComponent::Clone() const
{
	return new CMeshComponent(*this);
}

#include "CMeshComponent.h"

#include "../Asset/Material/CMaterial.h"
#include "../Asset/Mesh/CMesh.h"
#include "../Asset/Shader/CCBufferTransform.h"
#include "../World/CWorld.h"
#include "../World/CCameraManager.h"
#include "../Asset/CAssetManager.h"
#include "../Asset/Mesh/CMeshManager.h"
#include "../Asset/Shader/CShaderManager.h"

void CMeshComponent::SetMesh(const std::weak_ptr<CMesh>& Mesh)
{
	this->Mesh = Mesh;

	if (auto Mesh = this->Mesh.lock())
	{
		MaterialSlot.clear();
		auto MeshSlotCount = Mesh->GetSlotCount();
		for (size_t i = 0; i < MeshSlotCount; ++i)
		{
			auto Slot = Mesh->GetSlot(i);
			MaterialSlot.emplace_back(std::shared_ptr<CMaterial>(Slot->Material->Clone()));
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
	MaterialSlot[SlotIndex]->SetBaseColor(r, g, b, a);
}

void CMeshComponent::SetMaterialBaseColor(int SlotIndex, int r, int g, int b, int a)
{
	MaterialSlot[SlotIndex]->SetBaseColor(r, g, b, a);
}

void CMeshComponent::SetMaterialBaseColor(int SlotIndex, const FVector4& Color)
{
	MaterialSlot[SlotIndex]->SetBaseColor(Color);
}

void CMeshComponent::SetMaterialOpacity(int SlotIndex, float Opacity)
{
	MaterialSlot[SlotIndex]->SetOpacity(Opacity);
}

void CMeshComponent::AddTexture(int SlotIdx, const std::weak_ptr<CTexture>& Texture, int Register, int ShaderBufferType,
	int Index)
{
	MaterialSlot[SlotIdx]->AddTexture(Texture, Register, ShaderBufferType, Index);
}

void CMeshComponent::AddTexture(int SlotIdx, const std::string& Key, int Register, int ShaderBufferType, int Index)
{
	MaterialSlot[SlotIdx]->AddTexture(Key, Register, ShaderBufferType, Index);
}

void CMeshComponent::AddTexture(int SlotIdx, const std::string& Key, const TCHAR* FileName, const std::string& PathName,
	int Register, int ShaderBufferType, int Index)
{
	MaterialSlot[SlotIdx]->AddTexture(Key, FileName, PathName, Register, ShaderBufferType, Index);
}

void CMeshComponent::AddTextureFullPath(int SlotIdx, const std::string& Key, const TCHAR* FullPath, int Register,
	int ShaderBufferType, int Index)
{
	MaterialSlot[SlotIdx]->AddTextureFullPath(Key, FullPath, Register, ShaderBufferType, Index);
}

void CMeshComponent::AddTextures(int SlotIdx, const std::string& Key, std::vector<const TCHAR*>& FileNames,
	const std::string& PathName, int Register, int ShaderBufferType, int Index)
{
	MaterialSlot[SlotIdx]->AddTextures(Key, FileNames, PathName, Register, ShaderBufferType, Index);
}

void CMeshComponent::AddTexturesFullPath(int SlotIdx, const std::string& Key, std::vector<const TCHAR*>& FullPaths,
	int Register, int ShaderBufferType, int Index)
{
	MaterialSlot[SlotIdx]->AddTexturesFullPath(Key, FullPaths, Register, ShaderBufferType, Index);
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

	auto MeshSlotCount = MaterialSlot.size();
	for (size_t i = 0; i < MeshSlotCount; i++)
	{
		if (MaterialSlot[i])
		{
			MaterialSlot[i]->UpdateConstantBuffer();
		}

		Mesh->Render(i);
	}

	CSceneComponent::Render();
}

CMeshComponent* CMeshComponent::Clone() const
{
	return new CMeshComponent(*this);
}

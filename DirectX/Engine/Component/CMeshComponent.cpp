#include "CMeshComponent.h"

#include "CAnimation2DComponent.h"
#include "../Asset/CAssetManager.h"
#include "../Asset/Material/CMaterial.h"
#include "../Asset/Mesh/CMesh.h"
#include "../Asset/Mesh/CMeshManager.h"
#include "../Asset/Shader/CCBufferAnimation2D.h"
#include "../Asset/Shader/CCBufferTransform.h"
#include "../Asset/Shader/CShaderManager.h"
#include "../Asset/Texture/CTextureManager.h"
#include "../World/CCameraManager.h"
#include "../World/CWorld.h"

std::shared_ptr<CCBufferAnimation2D> CMeshComponent::EmptyAnimationCBuffer;

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
	if (auto World = this->World.lock())
	{
		if (auto WorldAssetMgr = World->GetWorldAssetManager().lock())
		{
			auto Mesh = WorldAssetMgr->FindMesh(Key);
			if (Mesh.expired())
			{
				return;
			}

			SetMesh(Mesh);
		}
	}
	else
	{
		auto InnerKey = "Mesh_" + Key;
		if (auto Mgr = CAssetManager::GetInst()->GetMeshManager().lock())
		{
			SetMesh(Mgr->FindMesh(InnerKey));
		}
	}
}

void CMeshComponent::SetShader(const std::string& Key)
{
	if (auto World = this->World.lock())
	{
		if (auto WorldAssetMgr = World->GetWorldAssetManager().lock())
		{
			auto Mesh = WorldAssetMgr->FindShader(Key);
			if (Mesh.expired())
			{
				return;
			}

			SetShader(Mesh);
		}
	}
	else
	{
		auto InnerKey = "Shader_" + Key;
		if (auto Mgr = CAssetManager::GetInst()->GetShaderManager().lock())
		{
			SetShader(Mgr->FindShader(InnerKey));
		}
	}
}

void CMeshComponent::SetBlendState(int SlotIndex, const std::string& Key)
{
	MaterialSlot[SlotIndex]->SetBlendState(Key);
}

void CMeshComponent::SetBlendState(int SlotIndex, const std::weak_ptr<CRenderState>& State)
{
	MaterialSlot[SlotIndex]->SetBlendState(State);
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
	std::weak_ptr<CTexture> Texture;
	if (auto World = this->World.lock())
	{
		if (auto WorldAssetMgr = World->GetWorldAssetManager().lock())
		{
			Texture = WorldAssetMgr->FindTexture(Key);
		}
	}
	else
	{
		auto InnerKey = "Texture_" + Key;
		if (auto Mgr = CAssetManager::GetInst()->GetTextureManager().lock())
		{
			Texture = Mgr->FindTexture(InnerKey);
		}
	}

	MaterialSlot[SlotIdx]->AddTexture(Texture, Register, ShaderBufferType, Index);
}

void CMeshComponent::AddTexture(int SlotIdx, const std::string& Key, const TCHAR* FileName, const std::string& PathName,
	int Register, int ShaderBufferType, int Index)
{
	std::weak_ptr<CTexture> Texture;
	if (auto World = this->World.lock())
	{
		if (auto WorldAssetMgr = World->GetWorldAssetManager().lock())
		{
			WorldAssetMgr->LoadTexture(Key, FileName, PathName);
			Texture = WorldAssetMgr->FindTexture(Key);
		}
	}
	else
	{
		auto InnerKey = "Texture_" + Key;
		if (auto Mgr = CAssetManager::GetInst()->GetTextureManager().lock())
		{
			Mgr->LoadTexture(InnerKey, FileName, PathName);
			Texture = Mgr->FindTexture(InnerKey);
		}
	}

	MaterialSlot[SlotIdx]->AddTexture(Texture, Register, ShaderBufferType, Index);
}

void CMeshComponent::AddTextureFullPath(int SlotIdx, const std::string& Key, const TCHAR* FullPath, int Register,
	int ShaderBufferType, int Index)
{
	std::weak_ptr<CTexture> Texture;
	if (auto World = this->World.lock())
	{
		if (auto WorldAssetMgr = World->GetWorldAssetManager().lock())
		{
			WorldAssetMgr->LoadTextureFullPath(Key, FullPath);
			Texture = WorldAssetMgr->FindTexture(Key);
		}
	}
	else
	{
		auto InnerKey = "Texture_" + Key;
		if (auto Mgr = CAssetManager::GetInst()->GetTextureManager().lock())
		{
			Mgr->LoadTextureFullPath(InnerKey, FullPath);
			Texture = Mgr->FindTexture(InnerKey);
		}
	}

	MaterialSlot[SlotIdx]->AddTexture(Texture, Register, ShaderBufferType, Index);
}

void CMeshComponent::AddTextures(int SlotIdx, const std::string& Key, std::vector<const TCHAR*>& FileNames,
	const std::string& PathName, int Register, int ShaderBufferType, int Index)
{
	std::weak_ptr<CTexture> Texture;
	if (auto World = this->World.lock())
	{
		if (auto WorldAssetMgr = World->GetWorldAssetManager().lock())
		{
			WorldAssetMgr->LoadTextures(Key, FileNames, PathName);
			Texture = WorldAssetMgr->FindTexture(Key);
		}
	}
	else
	{
		auto InnerKey = "Texture_" + Key;
		if (auto Mgr = CAssetManager::GetInst()->GetTextureManager().lock())
		{
			Mgr->LoadTextures(InnerKey, FileNames, PathName);
			Texture = Mgr->FindTexture(InnerKey);
		}
	}

	MaterialSlot[SlotIdx]->AddTexture(Texture, Register, ShaderBufferType, Index);
}

void CMeshComponent::AddTexturesFullPath(int SlotIdx, const std::string& Key, std::vector<const TCHAR*>& FullPaths,
	int Register, int ShaderBufferType, int Index)
{
	std::weak_ptr<CTexture> Texture;
	if (auto World = this->World.lock())
	{
		if (auto WorldAssetMgr = World->GetWorldAssetManager().lock())
		{
			WorldAssetMgr->LoadTexturesFullPath(Key, FullPaths);
			Texture = WorldAssetMgr->FindTexture(Key);
		}
	}
	else
	{
		auto InnerKey = "Texture_" + Key;
		if (auto Mgr = CAssetManager::GetInst()->GetTextureManager().lock())
		{
			Mgr->LoadTexturesFullPath(InnerKey, FullPaths);
			Texture = Mgr->FindTexture(InnerKey);
		}
	}

	MaterialSlot[SlotIdx]->AddTexture(Texture, Register, ShaderBufferType, Index);
}

bool CMeshComponent::SetTexture(int SlotIndex, int TextureIndex, const std::weak_ptr<CTexture>& Texture)
{
	if (static_cast<int>(MaterialSlot.size()) <= SlotIndex)
	{
		return false;
	}

	return MaterialSlot[SlotIndex]->SetTexture(TextureIndex, Texture);
}

bool CMeshComponent::SetTextureIndex(int SlotIndex, int TextureIndex)
{
	if (static_cast<int>(MaterialSlot.size()) <= SlotIndex)
	{
		return false;
	}

	return MaterialSlot[SlotIndex]->SetTextureIndex(TextureIndex);
}

void CMeshComponent::CreateEmptyAnimationCBuffer()
{
	EmptyAnimationCBuffer.reset(new CCBufferAnimation2D);

	EmptyAnimationCBuffer->Init();

	EmptyAnimationCBuffer->SetEnableAnimation2D(false);
}

void CMeshComponent::ClearEmptyAnimationCBuffer()
{
	EmptyAnimationCBuffer.reset();
}

bool CMeshComponent::Init()
{
	if (!CSceneComponent::Init())
	{
		return false;
	}

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

	auto Anim = AnimationComponent.lock();
	if (Anim)
	{
		Anim->SetShader();
	}
	else
	{
		EmptyAnimationCBuffer->UpdateBuffer();
	}

	auto MeshSlotCount = MaterialSlot.size();
	for (size_t i = 0; i < MeshSlotCount; i++)
	{
		auto& Material = MaterialSlot[i];
		if (Material)
		{
			if (Anim)
			{
				if (Anim->GetTextureType() == EAnimation2DTextureType::SpriteSheet)
				{
					Material->UpdateConstantBuffer();
				}
				else
				{
					Material->UpdateConstantBuffer(Anim->GetCurrentFrame());
				}
			}
			else
			{
				Material->UpdateConstantBuffer();
			}
		}

		Mesh->Render(i);

		if (Material)
		{
			Material->Reset();
		}
	}

	CSceneComponent::Render();
}

CMeshComponent* CMeshComponent::Clone() const
{
	return new CMeshComponent(*this);
}

CMeshComponent::CMeshComponent()
{
	RenderType = EComponentRender::Render;
}

CMeshComponent::CMeshComponent(const CMeshComponent& other): CSceneComponent(other),
                                                             Shader(other.Shader),
                                                             Mesh(other.Mesh),
                                                             CBufferTransform(other.CBufferTransform->Clone())
{
}

CMeshComponent::CMeshComponent(CMeshComponent&& other) noexcept: CSceneComponent(std::move(other)),
                                                                 Shader(std::move(other.Shader)),
                                                                 Mesh(std::move(other.Mesh)),
                                                                 CBufferTransform(std::move(other.CBufferTransform))
{
	other.Shader.reset();
	other.Mesh.reset();
	other.CBufferTransform.reset();
}

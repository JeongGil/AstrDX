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
#include "../Asset/Material/CMaterial.h"

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
		if (auto Mgr = CAssetManager::GetInst()->GetMeshManager().lock())
		{
			SetMesh(Mgr->FindMesh(Key));
		}
	}
}

void CMeshComponent::SetShader(const std::string& Key)
{
	if (auto Mgr = CAssetManager::GetInst()->GetShaderManager().lock())
	{
		if (auto Shader = Mgr->FindShader(Key).lock())
		{
			this->Shader = Shader;
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

std::weak_ptr<FMaterialTextureInfo> CMeshComponent::AddTexture(int SlotIdx, const std::weak_ptr<CTexture>& Texture,
                                                               int Register, int ShaderBufferType,
                                                               int Index)
{
	return MaterialSlot[SlotIdx]->AddTexture(Texture, Register, ShaderBufferType, Index);
}

std::weak_ptr<FMaterialTextureInfo> CMeshComponent::AddTexture(int SlotIdx, const std::string& Key, int Register,
                                                               int ShaderBufferType, int Index)
{
	if (auto World = this->World.lock())
	{
		if (auto Mgr = World->GetWorldAssetManager().lock())
		{
			if (auto Tex = Mgr->FindTexture(Key).lock())
			{
				return MaterialSlot[SlotIdx]->AddTexture(Tex, Register, ShaderBufferType, Index);
			}
		}
	}

	return std::weak_ptr<FMaterialTextureInfo>();
}

std::weak_ptr<FMaterialTextureInfo> CMeshComponent::AddTexture(int SlotIdx, const std::string& Key, const TCHAR* FileName, const std::string& PathName,
	int Register, int ShaderBufferType, int Index)
{
	if (auto World = this->World.lock())
	{
		if (auto Mgr = World->GetWorldAssetManager().lock())
		{
			if (!Mgr->LoadTexture(Key, FileName, PathName))
			{
				return std::weak_ptr<FMaterialTextureInfo>();
			}

			if (auto Tex = Mgr->FindTexture(Key).lock())
			{
				return MaterialSlot[SlotIdx]->AddTexture(Tex, Register, ShaderBufferType, Index);
			}
		}
	}

	return std::weak_ptr<FMaterialTextureInfo>();
}

std::weak_ptr<FMaterialTextureInfo> CMeshComponent::AddTextureFullPath(int SlotIdx, const std::string& Key,
                                                                       const TCHAR* FullPath, int Register,
                                                                       int ShaderBufferType, int Index)
{
	if (auto World = this->World.lock())
	{
		if (auto Mgr = World->GetWorldAssetManager().lock())
		{
			if (!Mgr->LoadTextureFullPath(Key, FullPath))
			{
				return std::weak_ptr<FMaterialTextureInfo>();
			}

			if (auto Tex = Mgr->FindTexture(Key).lock())
			{
				return MaterialSlot[SlotIdx]->AddTexture(Tex, Register, ShaderBufferType, Index);
			}
		}
	}

	return std::weak_ptr<FMaterialTextureInfo>();
}

std::weak_ptr<FMaterialTextureInfo> CMeshComponent::AddTextures(int SlotIdx, const std::string& Key,
                                                                std::vector<const TCHAR*>& FileNames,
                                                                const std::string& PathName, int Register,
                                                                int ShaderBufferType, int Index)
{
	if (auto World = this->World.lock())
	{
		if (auto Mgr = World->GetWorldAssetManager().lock())
		{
			if (!Mgr->LoadTextures(Key, FileNames, PathName))
			{
				return std::weak_ptr<FMaterialTextureInfo>();
			}

			if (auto Tex = Mgr->FindTexture(Key).lock())
			{
				return MaterialSlot[SlotIdx]->AddTexture(Tex, Register, ShaderBufferType, Index);
			}
		}
	}

	return std::weak_ptr<FMaterialTextureInfo>();
}

std::weak_ptr<FMaterialTextureInfo> CMeshComponent::AddTexturesFullPath(int SlotIdx, const std::string& Key, std::vector<const TCHAR*>& FullPaths,
	int Register, int ShaderBufferType, int Index)
{
	if (auto World = this->World.lock())
	{
		if (auto Mgr = World->GetWorldAssetManager().lock())
		{
			if (!Mgr->LoadTexturesFullPath(Key, FullPaths))
			{
				return std::weak_ptr<FMaterialTextureInfo>();
			}

			if (auto Tex = Mgr->FindTexture(Key).lock())
			{
				return MaterialSlot[SlotIdx]->AddTexture(Tex, Register, ShaderBufferType, Index);
			}
		}
	}

	return std::weak_ptr<FMaterialTextureInfo>();
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
			ProjMat = CamMgr->GetProjMatrix();
		}
	}

	CBufferTransform->SetWorldMatrix(WorldMatrix);
	CBufferTransform->SetViewMatrix(ViewMat);
	CBufferTransform->SetProjMatrix(ProjMat);

	FVector PivotSize = Pivot * Mesh->GetMeshSize();

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

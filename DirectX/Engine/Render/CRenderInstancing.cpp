#include "CRenderInstancing.h"

#include "CRenderState.h"
#include "../Utils.h"
#include "../Asset/CAssetManager.h"
#include "../Asset/Mesh/CMesh.h"
#include "../Asset/Shader/CShader.h"
#include "../Asset/Shader/CShaderManager.h"
#include "../Component/CAnimation2DComponent.h"
#include "../Component/CSceneComponent.h"
#include "../World/CWorld.h"

bool SortYRenderList(const std::weak_ptr<CSceneComponent>& A, const std::weak_ptr<CSceneComponent>& B)
{
	auto Src = A.lock();
	auto Dest = B.lock();

	if (!Src)
	{
		return true;
	}

	if (!Dest)
	{
		return false;
	}

	return Src->GetWorldPosition().y > Dest->GetWorldPosition().y;
}
CRenderInstancing::CRenderInstancing()
{
}

CRenderInstancing::~CRenderInstancing()
{
}

bool CRenderInstancing::CompareAsset(const std::weak_ptr<CMesh>& Mesh, const std::weak_ptr<CTexture>& Texture)
{
	return IsSameTarget(this->Mesh, Mesh) && IsSameTarget(this->Texture, Texture);
}

bool CRenderInstancing::CheckMesh(const std::weak_ptr<CMesh>& Mesh) const
{
	return IsSameTarget(this->Mesh, Mesh);
}

bool CRenderInstancing::CheckTexture(const std::weak_ptr<CTexture>& Texture) const
{
	return IsSameTarget(this->Texture, Texture);
}

void CRenderInstancing::SetMesh(const std::weak_ptr<CMesh>& Mesh)
{
	this->Mesh = Mesh;
}

void CRenderInstancing::SetTexture(const std::weak_ptr<CTexture>& Texture)
{
	this->Texture = Texture;
}

void CRenderInstancing::AddRenderComponent(const std::weak_ptr<CSceneComponent>& Component)
{
	auto Comp = Component.lock();
	if (!Comp || !Comp->GetAlive() || !Comp->GetEnable())
	{
		return;
	}

	if (RenderComponents.empty())
	{
		World = Comp->GetWorld();
		BlendState = Comp->GetBlendState();

		if (auto Shader = Comp->GetShader().lock())
		{
			std::string Key = Shader->GetKey() + "Instancing";
			if (auto ShaderMgr = CAssetManager::GetInst()->GetShaderManager().lock())
			{
				this->Shader = ShaderMgr->FindShader(Key);
			}
		}
	}

	RenderComponents.push_back(Component);

	std::erase_if(RenderComponents, [](const std::weak_ptr<CSceneComponent>& Weak)
		{
			auto Comp = Weak.lock();
			if (!Comp || !Comp->GetAlive())
			{
				return true;
			}

			return false;
		});

	if (!bRender)
	{
		if (RenderComponents.size() >= INSTANCING_THRESHOLD)
		{
			bRender = true;

			auto Mesh = this->Mesh.lock();
			if (Mesh && !Mesh->IsInstancingBufferCreated())
			{
				Mesh->CreateInstancingBuffer(sizeof(FInstancingBuffer), INSTANCING_THRESHOLD);
			}

			auto RenderSettingCandidates = RenderComponents
				| std::views::transform([](auto& Weak) {return Weak.lock(); })
				| std::views::filter([](const auto& Comp) {return Comp != nullptr; });

			for (const auto& Candidate : RenderSettingCandidates)
			{
				Candidate->SetRenderOption(EComponentRenderOption::Instancing);
			}
		}
	}
	else if (RenderComponents.size() < INSTANCING_THRESHOLD)
	{
		bRender = false;

		auto RenderSettingCandidates = RenderComponents
			| std::views::transform([](auto& Weak) {return Weak.lock(); })
			| std::views::filter([](const auto& Comp) {return Comp != nullptr; });

		for (const auto& Candidate : RenderSettingCandidates)
		{
			Candidate->SetRenderOption(EComponentRenderOption::Normal);
		}
	}
	else
	{
		Comp->SetRenderOption(EComponentRenderOption::Instancing);
	}
}

void CRenderInstancing::Render()
{
	if (!bRender || RenderComponents.empty())
	{
		return;
	}

	if (RenderComponents.size() > 1)
	{
		RenderComponents.sort(SortYRenderList);
	}

	auto BlendState = this->BlendState.lock();
	if (BlendState)
	{
		BlendState->SetState();
	}

	InstancingBuffers.clear();
	InstancingBuffers.resize(RenderComponents.size());

	int InstancingCount{ 0 };
	auto World = this->World.lock();
	auto CamMgr = World->GetCameraManager().lock();

	auto Mesh = this->Mesh.lock();
	auto Shader = this->Shader.lock();
	auto Texture = this->Texture.lock();

	auto RenderCompView = RenderComponents
		| std::views::transform([](const auto& Weak) {return Weak.lock(); })
		| std::views::filter([](const auto& Comp) {return Comp != nullptr; })
		| std::views::filter([](const auto& Comp) {return Comp->GetAlive(); })
		| std::views::filter([](const auto& Comp) {return Comp->GetEnable(); });

	for (const auto& Comp : RenderCompView)
	{
		FMatrix	ScaleMat, RotMat, TranslateMat;

		ScaleMat.Scaling(Comp->GetWorldScale());
		RotMat.Rotation(Comp->GetWorldRotation());
		TranslateMat.Translation(Comp->GetWorldPosition());

		FMatrix WorldMat = ScaleMat * RotMat * TranslateMat;

		FMatrix	ViewMat = CamMgr->GetViewMatrix();
		FMatrix	ProjMat = CamMgr->GetProjMatrix();

		FMatrix	WVPMat = WorldMat * ViewMat * ProjMat;

		auto& InstancingBuffer = InstancingBuffers[InstancingCount];
		InstancingBuffer.WVP0 = WVPMat[0];
		InstancingBuffer.WVP1 = WVPMat[1];
		InstancingBuffer.WVP2 = WVPMat[2];
		InstancingBuffer.WVP3 = WVPMat[3];

		if (auto AnimComp = Comp->GetAnimComponent().lock())
		{
			InstancingBuffer.LTUV = AnimComp->GetAnimLTUV();
			InstancingBuffer.RBUV = AnimComp->GetAnimRBUV();
		}
		else
		{
			InstancingBuffer.LTUV = FVector2(0.f, 0.f);
			InstancingBuffer.RBUV = FVector2(1.f, 1.f);
		}

		FVector Pivot = Comp->GetPivot();
		FVector PivotSize = Pivot * Mesh->GetMeshSize();

		InstancingBuffer.PivotSize = PivotSize;
		InstancingBuffer.BaseColor = Comp->GetBaseColor();

		++InstancingCount;

		Comp->SetRenderOption(EComponentRenderOption::Normal);
	}

	Mesh->SetInstancingData(&InstancingBuffers[0], InstancingCount);

	if (Texture)
	{
		Texture->SetShader(0, EShaderBufferType::Pixel, 0);
	}

	Shader->SetShader();
	Mesh->RenderInstancing();

	if (BlendState)
	{
		BlendState->ResetState();
	}

	RenderComponents.clear();
}

void CRenderInstancing::RenderClear()
{
	RenderComponents.clear();
}

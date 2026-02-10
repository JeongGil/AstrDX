#include "CRenderInstancing.h"

#include "CRenderState.h"
#include "../Utils.h"
#include "../Component/CSceneComponent.h"

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
	RenderComponents.push_back(Component);

	if (!bRender)
	{
		if (RenderComponents.size() >= 5)
		{
			bRender = true;

			auto Comps = RenderComponents
				| std::views::transform([](auto& Weak) {return Weak.lock(); });

			for (const auto& Comp : Comps)
			{
				Comp->SetRenderOption(EComponentRenderOption::Instancing);
			}
		}
	}
	else if (RenderComponents.size() < 5)
	{
		bRender = false;

		auto Comps = RenderComponents
			| std::views::transform([](auto& Weak) {return Weak.lock(); });

		for (const auto& Comp : Comps)
		{
			Comp->SetRenderOption(EComponentRenderOption::Normal);
		}
	}
	else
	{
		auto Comp = Component.lock();
		Comp->SetRenderOption(EComponentRenderOption::Instancing);
	}
}

void CRenderInstancing::Render()
{
	if (!bRender)
	{
		return;
	}

	if (RenderComponents.size() > 1)
	{
		RenderComponents.sort(SortYRenderList);
	}

	std::shared_ptr<CRenderState> BlendState;

	auto Comps = RenderComponents
		| std::views::transform([](auto& Weak) {return Weak.lock(); })
		| std::views::filter([](const auto& Comp) {return Comp != nullptr; });
	for (const auto& Comp : Comps)
	{
		BlendState = Comp->GetBlendState().lock();
	}

	if (BlendState)
	{
		BlendState->SetState();
	}

	if (BlendState)
	{
		BlendState->ResetState();
	}

	RenderComponents.clear();
}

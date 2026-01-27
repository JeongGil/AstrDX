#include "CRenderManager.h"

#include <ranges>

#include "CBlendState.h"
#include "CDepthStencilState.h"
#include "CRenderState.h"
#include "../Component/CSceneComponent.h"
#include "../Object/CGameObject.h"
#include "../World/CWorldManager.h"

static bool SortYRenderList(const std::weak_ptr<CSceneComponent>& A, const std::weak_ptr<CSceneComponent>& B)
{
	auto SA = A.lock();
	if (!SA)
	{
		return true;
	}

	auto SB = B.lock();
	if (!SB)
	{
		return false;
	}
	// Descend to higher Y draw first.
	return SA->GetWorldPosition().y > SB->GetWorldPosition().y;
}

bool CRenderManager::CreateLayer(const std::string& Name, int RenderOrder, ERenderListSort SortType)
{
	if (RenderLayers.contains(RenderOrder))
	{
		return false;
	}

	RenderLayers.emplace(RenderOrder, FRenderLayer{ .Name = Name, .SortType = SortType });

	return true;
}

void CRenderManager::AddRenderLayer(const std::weak_ptr<CSceneComponent>& Component)
{
	auto RenderComponent = Component.lock();
	if (!RenderComponent)
	{
		return;
	}

	auto It = RenderLayers.find(RenderComponent->GetRenderLayer());
	if (It == RenderLayers.end())
	{
		return;
	}

	It->second.RenderList.push_back(Component);
}

bool CRenderManager::TrySetRenderLayer(int Old, int New, const std::weak_ptr<CSceneComponent>& WeakComponent)
{
	if (Old == New)
	{
		return true;
	}

	auto Component = WeakComponent.lock();
	if (!Component)
	{
		return false;
	}

	auto OldLayerIt = RenderLayers.find(Old);
	if (OldLayerIt == RenderLayers.end())
	{
		return false;
	}

	auto& OldRenderList = OldLayerIt->second.RenderList;
	bool bFound = false;

	OldRenderList.erase(std::ranges::remove_if(OldRenderList,
		[&Component, &bFound](const std::weak_ptr<CSceneComponent>& Weak)
		{
			auto Locked = Weak.lock();
			if (!Locked)
			{
				return true;
			}

			if (Locked == Component)
			{
				bFound = true;
				return true;
			}

			return false;
		}).begin(), OldRenderList.end());

	if (!bFound)
	{
		return false;
	}

	auto [NewLayerIt, bInserted] = RenderLayers.try_emplace(New, FRenderLayer{ .Name = std::to_string(New) });
	NewLayerIt->second.RenderList.emplace_back(WeakComponent);

	return true;
}

void CRenderManager::ClearRenderLayer(int LayerOrder)
{
	for (auto& RenderLayer : RenderLayers | std::views::values)
	{
		RenderLayer.RenderList.clear();
	}
}

void CRenderManager::AddBlendRenderTargetDesc(const std::string& Key, bool BlendEnable, D3D11_BLEND SrcBlend,
	D3D11_BLEND DestBlend, D3D11_BLEND_OP BlendOp, D3D11_BLEND SrcBlendAlpha, D3D11_BLEND DestBlendAlpha,
	D3D11_BLEND_OP BlendOpAlpha, UINT8 RenderTargetWriteMask)
{
	std::shared_ptr<CBlendState> BlendState;

	auto It = RenderStates.find(Key);
	if (It == RenderStates.end())
	{
		std::shared_ptr<CRenderState> State(new CBlendState);
		RenderStates.emplace(Key, State);
		BlendState = std::dynamic_pointer_cast<CBlendState>(State);
	}
	else
	{
		BlendState = std::dynamic_pointer_cast<CBlendState>(It->second);
	}

	BlendState->AddRenderTargetDesc(
		BlendEnable,
		SrcBlend,
		DestBlend,
		BlendOp,
		SrcBlendAlpha,
		DestBlendAlpha,
		BlendOpAlpha,
		RenderTargetWriteMask);
}

void CRenderManager::SetBlendFactor(const std::string& Key, float r, float g, float b, float a)
{
	std::shared_ptr<CBlendState> BlendState;

	auto It = RenderStates.find(Key);
	if (It == RenderStates.end())
	{
		std::shared_ptr<CRenderState> State(new CBlendState);
		RenderStates.emplace(Key, State);
		BlendState = std::dynamic_pointer_cast<CBlendState>(State);
	}
	else
	{
		BlendState = std::dynamic_pointer_cast<CBlendState>(It->second);
	}

	BlendState->SetBlendFactor(r, g, b, a);
}

void CRenderManager::SetBlendSampleMask(const std::string& Key, UINT SampleMask)
{
	std::shared_ptr<CBlendState> BlendState;

	auto It = RenderStates.find(Key);
	if (It == RenderStates.end())
	{
		std::shared_ptr<CRenderState> State(new CBlendState);
		RenderStates.emplace(Key, State);
		BlendState = std::dynamic_pointer_cast<CBlendState>(State);
	}
	else
	{
		BlendState = std::dynamic_pointer_cast<CBlendState>(It->second);
	}

	BlendState->SetSampleMask(SampleMask);
}

bool CRenderManager::CreateBlendState(const std::string& Key, bool AlphaToCoverageEnable, bool IndependentBlendEnable)
{
	std::shared_ptr<CBlendState> BlendState;

	auto It = RenderStates.find(Key);
	if (It == RenderStates.end())
	{
		std::shared_ptr<CRenderState> State(new CBlendState);
		RenderStates.emplace(Key, State);
		BlendState = std::dynamic_pointer_cast<CBlendState>(State);
	}
	else
	{
		BlendState = std::dynamic_pointer_cast<CBlendState>(It->second);
	}

	if (!BlendState->CreateState(AlphaToCoverageEnable, IndependentBlendEnable))
	{
		RenderStates.erase(Key);

		return false;
	}

	return true;
}

bool CRenderManager::CreateDepthStencilState(const std::string& Key, bool DepthEnable,
	D3D11_DEPTH_WRITE_MASK DepthWriteMask, D3D11_COMPARISON_FUNC DepthFunc, bool StencilEnable, UINT8 StencilReadMask,
	UINT8 StencilWriteMask, D3D11_DEPTH_STENCILOP_DESC FrontFace, D3D11_DEPTH_STENCILOP_DESC BackFace)
{
	std::shared_ptr<CDepthStencilState> DepthState;

	auto It = RenderStates.find(Key);
	if (It == RenderStates.end())
	{
		std::shared_ptr<CRenderState> State(new CDepthStencilState);
		RenderStates.emplace(Key, State);
		DepthState = std::dynamic_pointer_cast<CDepthStencilState>(State);
	}
	else
	{
		DepthState = std::dynamic_pointer_cast<CDepthStencilState>(It->second);
	}

	if (!DepthState->CreateState(
		DepthEnable,
		DepthWriteMask,
		DepthFunc,
		StencilEnable,
		StencilReadMask,
		StencilWriteMask,
		FrontFace,
		BackFace))
	{
		RenderStates.erase(Key);

		return false;
	}

	return true;
}

void CRenderManager::SetState(const std::string& Key)
{
	auto It = RenderStates.find(Key);
	if (It == RenderStates.end())
	{
		return;
	}

	It->second->SetState();
}

void CRenderManager::ResetState(const std::string& Key)
{
	auto It = RenderStates.find(Key);
	if (It == RenderStates.end())
	{
		return;
	}

	It->second->ResetState();
}

std::weak_ptr<CRenderState> CRenderManager::FindRenderState(const std::string& Key)
{
	auto It = RenderStates.find(Key);
	if (It == RenderStates.end())
	{
		return std::weak_ptr<CRenderState>();
	}
	else
	{
		return It->second;
	}
}

bool CRenderManager::Init()
{
	AddBlendRenderTargetDesc("AlphaBlend", true,
		D3D11_BLEND_SRC_ALPHA,
		D3D11_BLEND_INV_SRC_ALPHA,
		D3D11_BLEND_OP_ADD);

	CreateBlendState("AlphaBlend");

	CreateDepthStencilState("DepthDisable", false);

	CreateLayer("Background", ERenderOrder::Background, ERenderListSort::Y);
	CreateLayer("Default", ERenderOrder::Default, ERenderListSort::Y);

	CreateLayer("DebugDraw", ERenderOrder::DebugDraw, ERenderListSort::Y);

	SetState("DepthDisable");

	return true;
}

void CRenderManager::Render()
{
	for (auto& RenderLayer : RenderLayers | std::views::values)
	{
		auto& RenderList = RenderLayer.RenderList;
		const auto EndIt = RenderList.end();

		auto It = RenderList.begin();
		while (It != EndIt)
		{
			if (It->expired())
			{
				It = RenderList.erase(It);

				continue;
			}

			++It;
		}

		switch (RenderLayer.SortType)
		{
			case ERenderListSort::None:
				break;
			case ERenderListSort::Y:
				if (!RenderList.empty())
				{
					RenderList.sort(SortYRenderList);
				}
				break;
			case ERenderListSort::Alpha:
				break;
		}

		for (const auto& WeakCmp : RenderList)
		{
			if (auto Cmp = WeakCmp.lock())
			{
				if (Cmp->GetEnable())
				{
					Cmp->Render();
					Cmp->PostRender();
				}
			}
		}
	}

	// UI
	SetState("AlphaBlend");

	CWorldManager::GetInst()->RenderUI();

	ResetState("AlphaBlend");
}

CRenderManager::~CRenderManager()
{
	ResetState("DepthDisable");
}

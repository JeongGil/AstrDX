#include "CRenderManager.h"

#include <ranges>

#include "CBlendState.h"
#include "CDepthStencilState.h"
#include "CPostProcessBlur.h"
#include "CRenderInstancing.h"
#include "CRenderState.h"
#include "../CDevice.h"
#include "../Asset/CAssetManager.h"
#include "../Asset/Mesh/CMeshManager.h"
#include "../Asset/Shader/CCBufferTransform.h"
#include "../Asset/Shader/CCBufferUIDefault.h"
#include "../Asset/Shader/CShader.h"
#include "../Asset/Shader/CShaderManager.h"
#include "../Asset/Texture/CRenderTarget.h"
#include "../Component/CSceneComponent.h"
#include "../Object/CGameObject.h"
#include "../World/CWorldManager.h"

bool CRenderManager::SortYRenderList(const std::weak_ptr<CSceneComponent>& A, const std::weak_ptr<CSceneComponent>& B)
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

bool CRenderManager::CreateLayer(const std::string& Name, int RenderOrder, ERenderListSort SortType)
{
	if (RenderLayers.contains(RenderOrder))
	{
		return false;
	}

	RenderLayers.emplace(RenderOrder, FRenderLayer{ .Name = Name, .SortType = SortType });

	return true;
}

int CRenderManager::GetLayerOrder(const std::string& Name)
{
	for (const auto& Pair : RenderLayers)
	{
		if (Pair.second.Name == Name)
		{
			return Pair.first;
		}
	}

	return -1;
}

void CRenderManager::SetBlurEnable(bool bEnable)
{
	Blur->SetEnable(bEnable);
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

std::weak_ptr<CRenderTarget> CRenderManager::FindRenderTarget(const std::string& Key)
{
	auto It = RenderTargets.find(Key);
	if (It == RenderTargets.end())
	{
		return {};
	}

	return It->second;
}

void CRenderManager::EnablePostProcess(const std::string& Key)
{
	for (const auto& PostProcess : PostProcesses)
	{
		if (PostProcess->GetKey() == Key)
		{
			PostProcess->SetEnable(true);
		}
	}
}

bool CRenderManager::CheckPostProcess(const std::string& Key)
{
	return std::ranges::any_of(PostProcesses, [&](const auto& PostProcess)
		{
			return PostProcess->GetKey() == Key;
		});
}

void CRenderManager::RenderFullScreenQuad()
{
	NullBufferShader.lock()->SetShader();

	auto Context = CDevice::GetInst()->GetContext();

	UINT Offset{ 0 };

	Context->IASetVertexBuffers(0, 0, nullptr, nullptr, &Offset);
	Context->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);

	Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	Context->Draw(4, 0);
}

void CRenderManager::CheckInstancing(const std::shared_ptr<CSceneComponent>& Comp, FRenderLayer& Layer)
{
	auto Mesh = Comp->GetMesh().lock();
	auto Tex = Comp->GetTexture().lock();
	auto Shader = Comp->GetShader().lock();

	if (!Mesh || !Shader)
	{
		return;
	}

	size_t TexID{ 0 };
	if (Tex)
	{
		TexID = Tex->GetID();
	}

	FRenderKey Key{ .MeshID = Mesh->GetID(), .ShaderID = Shader->GetID(), .TextureID = TexID };

	auto [Begin, End] = Layer.InstancingMap.equal_range(Key);

	std::shared_ptr<CRenderInstancing> Instancing;

	for (auto It = Begin; It != End; ++It)
	{
		auto& Inst = It->second;
		if (Inst->CheckMesh(Comp->GetMesh())
			&& Inst->CheckTexture(Comp->GetTexture()))
		{
			Instancing = Inst;
			break;
		}
	}

	if (!Instancing)
	{
		Instancing.reset(new CRenderInstancing);
		Instancing->SetMesh(Comp->GetMesh());
		Instancing->SetTexture(Comp->GetTexture());
	}

	Instancing->AddRenderComponent(Comp);
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
		D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA,
		D3D11_BLEND_OP_ADD,
		D3D11_BLEND_ONE, D3D11_BLEND_INV_SRC1_ALPHA);

	CreateBlendState("AlphaBlend");

	CreateDepthStencilState("DepthDisable", false);

	CreateLayer("Background", ERenderOrder::Background, ERenderListSort::None);
	CreateLayer("Map", ERenderOrder::Map, ERenderListSort::None);
	CreateLayer("Default", ERenderOrder::Default, ERenderListSort::Y);

	CreateLayer("DebugDraw", ERenderOrder::DebugDraw, ERenderListSort::Y);

	SetState("DepthDisable");

	FResolution	RS = CDevice::GetInst()->GetResolution();
	auto Target = CRenderTarget::Create("MainTarget", RS.Width, RS.Height, DXGI_FORMAT_R8G8B8A8_UNORM, true);
	RenderTargets.emplace("MainTarget", Target);

	Target = CRenderTarget::Create("FinalTarget", RS.Width, RS.Height, DXGI_FORMAT_R8G8B8A8_UNORM, true);
	RenderTargets.emplace("FinalTarget", Target);

	NullBufferShader = CAssetManager::GetInst()->GetShaderManager().lock()->FindShader("NullBuffer");

	Blur = CreatePostProcess<CPostProcessBlur>("Blur", 10).lock();
	Blur->SetEnable(false);

	if (auto MeshMgr = CAssetManager::GetInst()->GetMeshManager().lock())
	{
		TargetMesh = MeshMgr->FindMesh("Mesh_UIRectTex").lock();
	}

	if (auto ShaderMgr = CAssetManager::GetInst()->GetShaderManager().lock())
	{
		TargetShader = ShaderMgr->FindShader("UIDefault").lock();
	}

	TargetCBuffer.reset(new CCBufferUIDefault);
	TargetCBuffer->Init();

	TargetCBuffer->SetWidgetColor(FColor::White);
	TargetCBuffer->SetAnimEnable(false);
	TargetCBuffer->SetBrushTint(FColor::White);
	TargetCBuffer->SetTextureEnable(true);

	TargetTR.reset(new CCBufferTransform);
	TargetTR->Init();

	return true;
}

void CRenderManager::Update(const float DeltaTime)
{
	if (MouseWidget[MouseState])
	{
		MouseWidget[MouseState]->Update(DeltaTime);
	}

	for (auto& RenderLayer : RenderLayers | std::views::values)
	{
		std::erase_if(RenderLayer.RenderList, [&](const std::weak_ptr<CSceneComponent>& Weak)
			{
				auto Comp = Weak.lock();
				if (!Comp || !Comp->GetAlive())
				{
					return true;
				}

				if (Comp->GetEnable())
				{
					CheckInstancing(Comp, RenderLayer);
				}

				return false;
			});

		for (const auto& Instancing : RenderLayer.InstancingMap | std::views::values)
		{
			Instancing->Update(DeltaTime);
		}
	}

	CSync _(&Crt);

	std::erase_if(PostProcesses, [](const auto& PostProcess)
		{
			return !PostProcess->GetActive();
		});

	for (const auto& PostProcess : PostProcesses)
	{
		if (PostProcess->GetEnable())
		{
			PostProcess->Update(DeltaTime);
		}
	}
}

void CRenderManager::Render()
{
	// 1. Scene Object Rendering (MainTarget)
	auto MainTarget = FindRenderTarget("MainTarget").lock();
	MainTarget->SetTarget();
	{
		for (auto& RenderLayer : RenderLayers | std::views::values)
		{
			auto& RenderList = RenderLayer.RenderList;

			std::erase_if(RenderList, [](const std::weak_ptr<CSceneComponent>& Weak)
				{
					auto Comp = Weak.lock();
					if (!Comp || !Comp->GetAlive())
					{
						return true;
					}

					return false;
				});

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

			auto RenderComps = RenderList
				| std::views::transform([](auto& Weak) { return Weak.lock(); })
				| std::views::filter([](const auto& Comp) { return Comp != nullptr; })
				| std::views::filter([](const auto& Comp) { return Comp->GetEnable(); })
				| std::views::filter([](const auto& Comp)
					{
						return Comp->GetRenderOption() != EComponentRenderOption::Instancing;
					});

			for (const auto& Comp : RenderComps)
			{
				Comp->Render();
				//Comp->PostRender();
			}

			for (const auto& Instancing : RenderLayer.InstancingMap | std::views::values)
			{
				Instancing->Render();
				Instancing->RenderClear();
			}
		}
	}

	MainTarget->ResetTarget();

	// 2. Post-Processing Chain
	auto FinalTarget = FindRenderTarget("FinalTarget").lock();

	{
		CSync _(&Crt);

		std::shared_ptr<CRenderTarget> LastProcessedTarget = nullptr;
		std::erase_if(PostProcesses, [](const auto& PP)
			{
				return !PP->GetActive();
			});

		if (!PostProcesses.empty())
		{
			std::weak_ptr<CRenderTarget> CurrentInput = MainTarget;

			for (const auto& PP : PostProcesses)
			{
				if (PP->GetEnable())
				{
					PP->SetBlendTarget(CurrentInput);
					PP->RenderPostProcess();

					CurrentInput = PP->GetTarget();
					LastProcessedTarget = CurrentInput.lock();
				}
			}
		}

		// 3. Final Composite
		if (LastProcessedTarget)
		{
			// If the post-process has been executed, the final output becomes FinalTarget
			FinalTarget = LastProcessedTarget;
		}
		else
		{
			// If there is no post-process or it has not been executed, copy MainTarget to FinalTarget
			FinalTarget->SetTarget();
			MainTarget->SetShader(0, EShaderBufferType::Pixel, 0);
			RenderFullScreenQuad();
			FinalTarget->ResetTarget();
		}
	}

	// 4. Output to BackBuffer & UI
	// Output FinalTarget to the screen (BackBuffer)
	FinalTarget->SetShader(0, EShaderBufferType::Pixel, 0);
	RenderFullScreenQuad();

	// UI
	SetState("AlphaBlend");

	CWorldManager::GetInst()->RenderUI();

	ResetState("AlphaBlend");

	// Print render target to debug.
	if (bDebugTarget)
	{
		FVector Pos, Scale(400.f, 200.f, 1.f);

		FMatrix	ScaleMat, TranslateMat, WorldMat;

		ScaleMat.Scaling(Scale);
		TranslateMat.Translation(Pos);

		WorldMat = ScaleMat * TranslateMat;

		TargetTR->SetWorldMatrix(WorldMat);
		TargetTR->SetProjMatrix(CWidget::GetProjMatrix());

		TargetTR->UpdateBuffer();

		TargetCBuffer->UpdateBuffer();

		MainTarget->SetShader(0, EShaderBufferType::Pixel, 0);

		TargetShader.lock()->SetShader();

		TargetMesh.lock()->Render();

		{
			CSync sync(&Crt);

			for (const auto& PostProcess : PostProcesses)
			{
				Pos.y += Scale.y;
				TranslateMat.Translation(Pos);

				WorldMat = ScaleMat * TranslateMat;

				TargetTR->SetWorldMatrix(WorldMat);
				TargetTR->SetProjMatrix(CWidget::GetProjMatrix());

				TargetTR->UpdateBuffer();

				TargetCBuffer->UpdateBuffer();

				if (auto PostTarget = PostProcess->GetTarget().lock())
				{
					PostTarget->SetShader(0, EShaderBufferType::Pixel, 0);
				}

				TargetShader.lock()->SetShader();

				TargetMesh.lock()->Render();
			}
		}
	}

	SetState("AlphaBlend");

	if (MouseWidget[MouseState])
	{
		MouseWidget[MouseState]->Render();
	}

	ResetState("AlphaBlend");
}

CRenderManager::CRenderManager()
{
	InitializeCriticalSection(&Crt);
}

CRenderManager::~CRenderManager()
{
	DeleteCriticalSection(&Crt);

	ResetState("DepthDisable");
}

void CRenderManager::SetMouseWidget(EMouseState::Type State, CMouseWidget* Widget)
{
	MouseWidget[State].reset(Widget);

	ShowCursor(FALSE);
}

#pragma once

#include "../EngineInfo.h"
#include "../UI/CMouseWidget.h"
#include "CPostProcess.h"
#include "../Sync.h"

class CRenderInstancing;
class CRenderTarget;
class CRenderState;
class CSceneComponent;
class CShader;
class CMesh;
class CCBufferUIDefault;

enum class ERenderListSort
{
	None,
	Y,
	Alpha,
};

struct FRenderLayer
{
	std::string Name;
	std::list<std::weak_ptr<CSceneComponent>> RenderList;
	std::unordered_multimap<FRenderKey, std::shared_ptr<CRenderInstancing>, FRenderKeyHash> InstancingMap;
	ERenderListSort SortType = ERenderListSort::Y;
};

class CRenderManager
{
public:
	static bool SortYRenderList(const std::weak_ptr<CSceneComponent>& A, const std::weak_ptr<CSceneComponent>& B);

	bool CreateLayer(const std::string& Name, int RenderOrder, ERenderListSort SortType);
	int GetLayerOrder(const std::string& Name);
	void AddRenderLayer(const std::weak_ptr<CSceneComponent>& Component);
	bool TrySetRenderLayer(int Old, int New, const std::weak_ptr<CSceneComponent>& WeakComponent);
	void ClearRenderLayer(int LayerOrder);

	void AddBlendRenderTargetDesc(const std::string& Key,
		bool BlendEnable = true,
		D3D11_BLEND SrcBlend = D3D11_BLEND_ONE,
		D3D11_BLEND DestBlend = D3D11_BLEND_ZERO,
		D3D11_BLEND_OP BlendOp = D3D11_BLEND_OP_ADD,
		D3D11_BLEND SrcBlendAlpha = D3D11_BLEND_ONE,
		D3D11_BLEND DestBlendAlpha = D3D11_BLEND_ZERO,
		D3D11_BLEND_OP BlendOpAlpha = D3D11_BLEND_OP_ADD,
		UINT8 RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL);
	void SetBlendFactor(const std::string& Key, float r, float g, float b, float a);
	void SetBlendSampleMask(const std::string& Key, UINT SampleMask);
	bool CreateBlendState(const std::string& Key,
		bool AlphaToCoverageEnable = false,
		bool IndependentBlendEnable = false);

	bool CreateDepthStencilState(const std::string& Key,
		bool DepthEnable = true,
		D3D11_DEPTH_WRITE_MASK DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL,
		D3D11_COMPARISON_FUNC DepthFunc = D3D11_COMPARISON_LESS,
		bool StencilEnable = false,
		UINT8 StencilReadMask = 0xFF,
		UINT8 StencilWriteMask = 0xFF,
		D3D11_DEPTH_STENCILOP_DESC FrontFace =
		{
			.StencilFailOp = D3D11_STENCIL_OP_KEEP,
			.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP,
			.StencilPassOp = D3D11_STENCIL_OP_KEEP,
			.StencilFunc = D3D11_COMPARISON_ALWAYS
		},
		D3D11_DEPTH_STENCILOP_DESC BackFace =
		{
			.StencilFailOp = D3D11_STENCIL_OP_KEEP,
			.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP,
			.StencilPassOp = D3D11_STENCIL_OP_KEEP,
			.StencilFunc = D3D11_COMPARISON_ALWAYS
		});

	void SetState(const std::string& Key);
	void ResetState(const std::string& Key);

	std::weak_ptr<CRenderTarget> FindRenderTarget(const std::string& Key);
	std::weak_ptr<CRenderState> FindRenderState(const std::string& Key);

	void EnablePostProcess(const std::string& Key);
	bool CheckPostProcess(const std::string& Key);

	bool Init();
	void Update(const float DeltaTime);
	void Render();

private:
	void RenderFullScreenQuad();
	void CheckInstancing(const std::shared_ptr<CSceneComponent>& Comp, FRenderLayer& Layer);

public:
	template <typename T>
	std::weak_ptr<T> SetMouseWidget(EMouseState::Type State, const std::string& Key)
	{
		CSync _(&Crt);

		auto& Widget = MouseWidget[State];
		Widget.reset(new T);

		Widget->SetKey(Key);

		if (!Widget->Init())
		{
			return {};
		}

		ShowCursor(FALSE);

		return std::dynamic_pointer_cast<T>(Widget);
	}

	template <typename T>
	std::weak_ptr<T> CreatePostProcess(const std::string& Key, int Order = 0)
	{
		CSync _(&Crt);

		std::shared_ptr<CPostProcess> Obj{ new T };

		Obj->SetKey(Key);
		Obj->SetOrder(Order);

		if (!Obj->Init())
		{
			return {};
		}

		PostProcesses.push_back(Obj);

		std::ranges::sort(PostProcesses, CRenderManager::SortPostProcess);

		return std::dynamic_pointer_cast<T>(Obj);
	}

private:
	CRenderManager();
	~CRenderManager();

public:
	static CRenderManager* GetInst()
	{
		if (!Inst)
		{
			Inst = new CRenderManager;
		}

		return Inst;
	}

	static void DestroyInst()
	{
		SAFE_DELETE(Inst);
	}

private:
	std::unordered_map<std::string, std::shared_ptr<CRenderState>> RenderStates;
	std::map<int, FRenderLayer> RenderLayers;
	std::vector<std::shared_ptr<CPostProcess>> PostProcesses;

	std::shared_ptr<CPostProcess> Blur;

	EMouseState::Type MouseState = EMouseState::Normal;
	std::shared_ptr<CMouseWidget> MouseWidget[EMouseState::End];

	std::unordered_map<std::string, std::shared_ptr<CRenderTarget>> RenderTargets;
	std::weak_ptr<CShader> NullBufferShader;

	std::weak_ptr<CMesh> TargetMesh;
	std::weak_ptr<CShader> TargetShader;
	std::shared_ptr<CCBufferUIDefault> TargetCBuffer;
	std::shared_ptr<CCBufferTransform> TargetTR;

	CRITICAL_SECTION Crt;

	inline static CRenderManager* Inst;

public:
	[[nodiscard]] EMouseState::Type GetMouseState() const
	{
		return MouseState;
	}

	void SetMouseWidget(EMouseState::Type State, CMouseWidget* Widget);

	void SetBlurEnable(bool bEnable);

private:
	static bool SortPostProcess(const std::shared_ptr<CPostProcess>& A, const std::shared_ptr<CPostProcess>& B)
	{
		return A->GetOrder() > B->GetOrder();
	}
};




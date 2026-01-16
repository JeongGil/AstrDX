#pragma once

#include "../EngineInfo.h"

class CRenderState;
class CSceneComponent;

struct FRenderLayer;

enum class ERenderListSort
{
	None,
	Y,
	Alpha,
};

class CRenderManager
{
public:
	bool CreateLayer(const std::string& Name, int RenderOrder, ERenderListSort SortType);
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

	std::weak_ptr<CRenderState> FindRenderState(const std::string& Key);

	bool Init();
	void Render();

private:
	CRenderManager() = default;
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

	inline static CRenderManager* Inst;
};

struct FRenderLayer
{
	std::string Name;
	ERenderListSort SortType = ERenderListSort::Y;
	std::list<std::weak_ptr<CSceneComponent>> RenderList;
};


#pragma once

#include "../EngineInfo.h"

class CShader;
class CTexture;
class CMesh;
class CSceneComponent;
class CRenderManager;
class CRenderState;
class CWorld;

class CRenderInstancing
{
	friend CRenderManager;

	static constexpr int INSTANCING_THRESHOLD{ 32 };
private:
	CRenderInstancing();

public:
	~CRenderInstancing();

private:
	std::list<std::weak_ptr<CSceneComponent>> RenderComponents;

	std::weak_ptr<CMesh> Mesh;
	std::weak_ptr<CTexture> Texture;
	std::weak_ptr<CShader> Shader;
	std::weak_ptr<CRenderState> BlendState;

	bool bRender{ false };
	std::vector<FInstancingBuffer> InstancingBuffers;

	std::weak_ptr<CWorld> World;

public:
	bool CompareAsset(const std::weak_ptr<CMesh>& Mesh, const std::weak_ptr<CTexture>& Texture);

	bool CheckMesh(const std::weak_ptr<CMesh>& Mesh) const;
	bool CheckTexture(const std::weak_ptr<CTexture>& Texture) const;

	void SetMesh(const std::weak_ptr<CMesh>& Mesh);
	void SetTexture(const std::weak_ptr<CTexture>& Texture);

	void AddRenderComponent(const std::weak_ptr<CSceneComponent>& Component);

	void Render();
	void RenderClear();

	size_t GetRenderCount() const
	{
		return RenderComponents.size();
	}
};


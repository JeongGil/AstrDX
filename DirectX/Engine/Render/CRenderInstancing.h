#pragma once

#include "../EngineInfo.h"

class CShader;
class CTexture;
class CMesh;
class CSceneComponent;
class CRenderManager;

class CRenderInstancing
{
	friend CRenderManager;

private:
	CRenderInstancing();

public:
	~CRenderInstancing();

private:
	std::list<std::weak_ptr<CSceneComponent>> RenderComponents;

	std::weak_ptr<CMesh> Mesh;
	std::weak_ptr<CTexture> Texture;
	std::weak_ptr<CShader> Shader;

public:
	bool CompareAsset(const std::weak_ptr<CMesh>& Mesh, const std::weak_ptr<CTexture>& Texture);
	
	void SetMesh(const std::weak_ptr<CMesh>& Mesh);
	void SetTexture(const std::weak_ptr<CTexture>& Texture);

	void AddRenderComponent(const std::weak_ptr<CSceneComponent>& Component);

	void Render();
};


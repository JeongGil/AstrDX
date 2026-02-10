#include "CRenderInstancing.h"

#include "../Utils.h"

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
}

void CRenderInstancing::Render()
{
}

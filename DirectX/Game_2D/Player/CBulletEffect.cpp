#include "CBulletEffect.h"

#include "Component/CMeshComponent.h"
#include "Component/CAnimation2DComponent.h"

void CBulletEffect::FinishAnimation()
{
	Destroy();
}

bool CBulletEffect::Init()
{
	if (!CGameObject::Init())
	{
		return false;
	}

	MeshComponent = CreateComponent<CMeshComponent>("Mesh");
	if (auto Mesh = MeshComponent.lock())
	{
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetWorldScale(100, 100);
		Mesh->SetBlendState(0, "AlphaBlend");
	}

	Animation2DComponent = CreateComponent<CAnimation2DComponent>("Animation2D");
	if (auto Anim =Animation2DComponent.lock())
	{
		Anim->SetUpdateComponent(MeshComponent);
		Anim->AddAnimation("Explosion", 3);
		Anim->SetFinishNotify<CBulletEffect>("Explosion", this, &CBulletEffect::FinishAnimation);
	}

	return true;
}

void CBulletEffect::Update(const float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

CBulletEffect* CBulletEffect::Clone()
{
	return new CBulletEffect(*this);
}

CBulletEffect::CBulletEffect(const CBulletEffect& other): CGameObject(other),
                                                          MeshComponent(other.MeshComponent),
                                                          Animation2DComponent(other.Animation2DComponent)
{
}

CBulletEffect::CBulletEffect(CBulletEffect&& other) noexcept: CGameObject(std::move(other)),
                                                              MeshComponent(std::move(other.MeshComponent)),
                                                              Animation2DComponent(std::move(other.Animation2DComponent))
{
}

CBulletEffect& CBulletEffect::operator=(const CBulletEffect& other)
{
	if (this == &other)
		return *this;
	CGameObject::operator =(other);
	MeshComponent = other.MeshComponent;
	Animation2DComponent = other.Animation2DComponent;
	return *this;
}

CBulletEffect& CBulletEffect::operator=(CBulletEffect&& other) noexcept
{
	if (this == &other)
		return *this;
	CGameObject::operator =(std::move(other));
	MeshComponent = std::move(other.MeshComponent);
	Animation2DComponent = std::move(other.Animation2DComponent);
	return *this;
}

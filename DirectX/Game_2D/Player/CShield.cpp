#include "CShield.h"

#include <Component/CMeshComponent.h>
#include <Component/CRotationMovementComponent.h>

bool CShield::Init()
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
		Mesh->SetRelativeScale(64, 64);

		Mesh->AddTexture(0, "Shield", TEXT("Icon_Shield.PNG"));
	}

	return true;
}

void CShield::Update(const float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CShield::PostUpdate(const float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CShield* CShield::Clone()
{
	return new CShield(*this);
}

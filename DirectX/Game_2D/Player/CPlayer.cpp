#include "CPlayer.h"

#include "Component/CMeshComponent.h"

bool CPlayer::Init()
{
	if (!CGameObject::Init())
	{
		return false;
	}

	MeshComponent = CreateComponent<CMeshComponent>("Mesh");
	if (auto Mesh = MeshComponent.lock())
	{
		Mesh->SetShader("Color2D");
		Mesh->SetMesh("CenterCubeColor");
	}

	return true;
}

void CPlayer::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	if (auto Mesh = MeshComponent.lock())
	{
		Mesh->AddRelativeRotationZ(30.f * DeltaTime);
	}
}

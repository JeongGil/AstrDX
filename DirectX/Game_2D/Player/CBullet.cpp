#include "CBullet.h"

#include "Component/CMeshComponent.h"

bool CBullet::Init()
{
	if (!CGameObject::Init())
	{
		return false;
	}

	MeshComponent = CreateComponent<CMeshComponent>("Mesh");
	if (auto Mesh = MeshComponent.lock())
	{
		Mesh->SetShader("Color2D");
		Mesh->SetMesh("CenterRectColor");
		Mesh->SetRelativeScale(50, 50);
	}

	return true;
}

void CBullet::Update(const float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	constexpr float Speed = 300;
	float DelDist = Speed * DeltaTime;
	FVector DelPos = GetAxis(EAxis::Y) * DelDist;
	Distance -= DelDist;

	AddWorldPosition(DelPos);

	if (Distance <= 0.f)
	{
		Destroy();
	}
}

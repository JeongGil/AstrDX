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
		if (GetAsyncKeyState('W') & 0x8000)
		{
			Mesh->AddRelativePosition(Mesh->GetAxis(EAxis::Y) * 2.f * DeltaTime);
		}

		if (GetAsyncKeyState('S') & 0x8000)
		{
			Mesh->AddRelativePosition(Mesh->GetAxis(EAxis::Y) * -2.f * DeltaTime);
		}

		if (GetAsyncKeyState('A') & 0x8000)
		{
			Mesh->AddRelativeRotationZ(30.f * DeltaTime);
		}

		if (GetAsyncKeyState('D') & 0x8000)
		{
			Mesh->AddRelativeRotationZ(-30.f * DeltaTime);
		}

		// CBullet 클래스를 만들고 플레이어의 Y축 위쪽으로 위치를 잡아서
		// 생성하고 CBullet Update에서는 Y축 방향으로 계속 이동하게
		// 만들어보자.
		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
		}
	}
}

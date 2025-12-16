#include "CPlayer.h"

#include <World/CWorld.h>

#include "CBullet.h"
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

	Rotation = CreateComponent<CSceneComponent>("Rot");
	if (auto RotCmp = Rotation.lock())
	{
		RotCmp->SetInheritRotation(false);
		RotCmp->SetInheritScale(false);
	}

	SubMeshComponent = CreateComponent<CMeshComponent>("Mesh", "Rotation");
	if (auto Mesh = SubMeshComponent.lock())
	{
		Mesh->SetShader("Color2D");
		Mesh->SetMesh("CenterCubeColor");

		Mesh->SetInheritScale(false);
		Mesh->SetRelativePosition(1.f, 0.f, 0.f);
		Mesh->SetRelativeScale(0.2f, 0.2f, 0.2f);
	}

	return true;
}

void CPlayer::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	if (auto RotCmp = Rotation.lock())
	{
		RotCmp->AddRelativeRotationZ(100.f * DeltaTime);
	}

	//if (auto SubMesh = SubMeshComponent.lock())
	//{
	//	SubMesh->AddRelativePosition(FVector(0.2f, 0, 0) * DeltaTime);
	//}

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

		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			if (auto World = this->World.lock())
			{
				const std::string BulletName = "Bullet_";

				static int Counter = 0;
				const auto WeakBullet = World->CreateGameObject<CBullet>(BulletName + std::to_string(Counter++));
				if (auto Bullet = WeakBullet.lock())
				{
					Bullet->SetWorldScale(0.3f, 0.3f, 0.3f);
					Bullet->SetWorldPosition(GetWorldPosition() + GetAxis(EAxis::Y) * 0.6f);
					Bullet->SetWorldRotation(GetWorldRotation());
				}
			}
		}
	}
}

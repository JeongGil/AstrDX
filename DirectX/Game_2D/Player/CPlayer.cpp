#include "CPlayer.h"

#include <World/CWorld.h>

#include "CBullet.h"
#include "CDevice.h"
#include "CMissile.h"
#include "Component/CMeshComponent.h"
#include "Component/CCameraComponent.h"
#include "../Monster/CMonster.h"
#include "../Component/CStateComponent.h"

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
		Mesh->SetMesh("CenterRectColor");
		Mesh->SetWorldScale(100, 100);
	}

	Rotation = CreateComponent<CSceneComponent>("Rotation");
	if (auto RotCmp = Rotation.lock())
	{
		RotCmp->SetInheritRotation(false);
		RotCmp->SetInheritScale(false);
	}

	SubMeshComponent = CreateComponent<CMeshComponent>("SubMesh", "Rotation");
	if (auto Mesh = SubMeshComponent.lock())
	{
		Mesh->SetShader("Color2D");
		Mesh->SetMesh("CenterRectColor");

		Mesh->SetInheritScale(false);
		Mesh->SetRelativePosition(100, 0, 0);
		Mesh->SetRelativeScale(50, 50);
	}

	CameraComponent = CreateComponent<CCameraComponent>("PlayerCamera");
	if (auto Cam = CameraComponent.lock())
	{
		const auto& Resolution = CDevice::GetInst()->GetResolution();
		Cam->SetProjection(CCameraComponent::EProjectionType::Orthogonal,
			90.f, static_cast<float>(Resolution.Width), static_cast<float>(Resolution.Height), 1000);

		Cam->SetInheritRotation(false);
	}

	StateComponent = CreateComponent<CStateComponent>("State");

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
			Mesh->AddRelativePosition(Mesh->GetAxis(EAxis::Y) * 100 * DeltaTime);
		}

		if (GetAsyncKeyState('S') & 0x8000)
		{
			Mesh->AddRelativePosition(Mesh->GetAxis(EAxis::Y) * -100 * DeltaTime);
		}

		if (GetAsyncKeyState('A') & 0x8000)
		{
			Mesh->AddRelativeRotationZ(180 * DeltaTime);
		}

		if (GetAsyncKeyState('D') & 0x8000)
		{
			Mesh->AddRelativeRotationZ(-180 * DeltaTime);
		}

		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			if (auto World = this->World.lock())
			{
				//const std::string BulletName = "Bullet_";
				//static int Counter = 0;
				const auto WeakBullet = World->CreateGameObject<CBullet>("Bullet");
				if (auto Bullet = WeakBullet.lock())
				{
					Bullet->SetWorldPosition(GetWorldPosition() + GetAxis(EAxis::Y) * 75);
					Bullet->SetWorldRotation(GetWorldRotation());
					Bullet->SetCollisionTargetName("Monster");
					Bullet->CalcCollisionRadius();
				}
			}
		}

		if (GetAsyncKeyState('1') & 0x8000)
		{
			if (auto World = this->World.lock())
			{
				//const std::string BulletName = "Missile_";
				//static int Counter = 0;
				auto WeakMissile = World->CreateGameObject<CMissile>("Missile");
				if (auto Missile = WeakMissile.lock())
				{
					Missile->SetWorldPosition(GetWorldPosition() + GetAxis(EAxis::Y) * 100);
					Missile->SetWorldRotation(GetWorldRotation());
					Missile->SetCamera();
				}
			}
		}

		if (GetAsyncKeyState('3') & 0x8000)
		{
			for (int i = 0; i < 12; i++)
			{
				if (auto World = this->World.lock())
				{
					auto WeakBullet = World->CreateGameObject<CBullet>("Bullet");
					if (auto Bullet = WeakBullet.lock())
					{
						FMatrix DirMat;
						DirMat.RotationZ(static_cast<float>(i) * 30.f);

						FVector BulletDir = GetAxis(EAxis::Y).TransformNormal(DirMat).GetNormalized();
						constexpr float Offset = 90.f;
						FVector BulletPos = GetWorldPosition() + BulletDir * Offset;

						Bullet->SetWorldPosition(BulletPos);
						Bullet->CalcCollisionRadius();

						Bullet->SetCloseTarget<CMonster>();
						Bullet->SetCollisionTargetName("Monster");
					}
				}
			}
		}
	}
}

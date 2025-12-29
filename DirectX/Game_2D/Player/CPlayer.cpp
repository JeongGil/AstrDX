#include "CPlayer.h"

#include <World/CWorld.h>

#include "CBullet.h"
#include "CDevice.h"
#include "CMissile.h"
#include <CEngine.h>
#include "Component/CMeshComponent.h"
#include "Component/CCameraComponent.h"
#include "../Monster/CMonster.h"
#include "../Component/CStateComponent.h"
#include "Component/CAnimation2DComponent.h"

void CPlayer::TestNotify()
{
	OutputDebugString(TEXT("Test Notify\n"));
}

void CPlayer::AttackNotify()
{
	if (auto World = this->World.lock())
	{
		auto WeakBullet = World->CreateGameObject<CBullet>("Bullet");
		if (auto Bullet = WeakBullet.lock())
		{
			Bullet->SetWorldPosition(GetWorldPosition() + GetAxis(EAxis::Y) * 75.f);
			Bullet->SetWorldRotation(GetWorldRotation());
			Bullet->SetCollisionTargetName("Monster");
			Bullet->CalcCollisionRadius();
		}
	}
}

void CPlayer::AttackFinish()
{
	bOnAttack = false;
	if (auto Anim = Animation2DComponent.lock())
	{
		Anim->ChangeAnimation("PlayerIdle");
	}
}

bool CPlayer::Init()
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

		Mesh->SetMaterialBaseColor(0, FColor(1, 0, 0, 0));
		Mesh->SetBlendState(0, "AlphaBlend");
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
		Mesh->SetShader("MaterialColor2D");
		Mesh->SetMesh("CenterRectColor");

		Mesh->SetInheritScale(false);
		Mesh->SetRelativePosition(100, 0, 0);
		Mesh->SetRelativeScale(50, 50);

		Mesh->SetMaterialBaseColor(0, FColor::Red);
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

	Animation2DComponent = CreateComponent<CAnimation2DComponent>("Animation2D");
	if (auto Anim = Animation2DComponent.lock())
	{
		Anim->SetUpdateComponent(MeshComponent);

		Anim->AddAnimation("PlayerIdle");
		Anim->AddAnimation("PlayerWalk");
		Anim->AddAnimation("PlayerAttack", 0.5f);
		//Anim->SetPlayRate("PlayerAttack", 2.f);
		//Anim->ChangeAnimation("PlayerWalk");

		Anim->AddNotify<CPlayer>("PlayerAttack", "AttackNotify", 2, this, &CPlayer::AttackNotify);
		Anim->SetFinishNotify<CPlayer>("PlayerAttack", this, &CPlayer::AttackFinish);

		Anim->SetLoop("PlayerIdle", true);
		Anim->SetLoop("PlayerWalk", true);
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

	if (auto SubMesh = SubMeshComponent.lock())
	{
		static float Elapsed = 0.f;
		Elapsed += DeltaTime;
		if (Elapsed >= 1.f)
		{
			Elapsed -= 1.f;
			auto& MT = CEngine::GetInst()->GetMT();
			std::uniform_real_distribution<float> Dist(0.f, 1.f);
			SubMesh->SetMaterialBaseColor(0, Dist(MT), Dist(MT), Dist(MT), 0.f);
		}
		//SubMesh->AddRelativePosition(FVector(0.2f, 0, 0) * DeltaTime);
	}

	if (auto Mesh = MeshComponent.lock())
	{
		auto Anim = Animation2DComponent.lock();

		bool bMove = false;

		if (GetAsyncKeyState('W') & 0x8000)
		{
			Mesh->AddRelativePosition(Mesh->GetAxis(EAxis::Y) * 100 * DeltaTime);
			Anim->ChangeAnimation("PlayerWalk");
			bMove = true;
		}

		if (GetAsyncKeyState('S') & 0x8000)
		{
			Mesh->AddRelativePosition(Mesh->GetAxis(EAxis::Y) * -100 * DeltaTime);
			Anim->ChangeAnimation("PlayerWalk");
			bMove = true;
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
			Anim->ChangeAnimation("PlayerAttack");
			bOnAttack = true;
		}

		if (!bMove && !bOnAttack)
		{
			Anim->ChangeAnimation("PlayerIdle");
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

void CPlayer::Destroy()
{
	CGameObject::Destroy();
}

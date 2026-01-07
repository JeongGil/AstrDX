#include "CMonster.h"

#include <CColliderBox2D.h>
#include "../Player/CBullet.h"
#include "Component/CAnimation2DComponent.h"
#include "Component/CColliderSphere2D.h"
#include "Component/CMeshComponent.h"
#include "World/CWorld.h"

bool CMonster::Init()
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
		Mesh->SetWorldScale(45.f, 60.f);
		Mesh->SetBlendState(0, "AlphaBlend");
	}

	Animation2DComponent = CreateComponent<CAnimation2DComponent>("Animation2D");
	if (auto Anim = Animation2DComponent.lock())
	{
		Anim->SetUpdateComponent(MeshComponent);

		Anim->AddAnimation("MonsterIdle", 2.8f);
		Anim->AddAnimation("MonsterAttack", 0.5f);

		Anim->AddNotify<CMonster>("MonsterAttack", "AttackNotify", 2, this, &CMonster::AttackNotify);
		Anim->SetFinishNotify<CMonster>("MonsterAttack", this, &CMonster::AttackFinish);

		Anim->SetLoop("MonsterIdle", true);
	}

	Body = CreateComponent<CColliderBox2D>("Body");
	if (auto Body = this->Body.lock())
	{
		Body->SetCollisionProfile("Monster");
		Body->SetBoxExtend(100.f, 100.f);
		Body->SetDrawDebug(true);
		Body->SetInheritScale(false);
	}

	if (auto World = this->World.lock())
	{
		FireTarget = World->FindObject<CGameObject>("Player");
	}

	return true;
}

void CMonster::Update(const float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	if (auto Target = FireTarget.lock())
	{
		FVector Displacement = Target->GetWorldPosition() - GetWorldPosition();

		bool bIsInRange = Displacement.SqrLength() <= DetectRange * DetectRange;

		// Look at target;
		if (bIsInRange)
		{
			float Degree = GetWorldPosition().GetViewTargetAngleDegree2D(Target->GetWorldPosition());

			SetWorldRotationZ(Degree);
		}

		ElapsedFromShot += DeltaTime;
		if (bIsInRange && ElapsedFromShot >= 1.f)
		{
			ElapsedFromShot = 0.f;

			if (auto Anim = Animation2DComponent.lock())
			{
				Anim->ChangeAnimation("MonsterAttack");
				bOnAttack = true;
			}
		}
	}
}

CMonster* CMonster::Clone()
{
	return new CMonster(*this);
}

void CMonster::AttackNotify()
{
	if (auto World = this->World.lock())
	{
		auto WeakBullet = World->CreateGameObject<CBullet>("Bullet");
		if (auto Bullet = WeakBullet.lock())
		{
			FVector Position = GetWorldPosition() + GetAxis(EAxis::Y) * 75.f;

			Bullet->SetCollision("MonsterAttack");
			Bullet->SetWorldPosition(Position);
			Bullet->SetWorldRotation(GetWorldRotation());
			Bullet->SetCollisionTargetName("Player");
			Bullet->CalcCollisionRadius();

			if (auto Target = FireTarget.lock())
			{
				FVector Dir = Target->GetWorldPosition() - Position;
				Dir.Normalize();

				Bullet->SetMoveDirection(Dir);
			}
		}
	}
}

void CMonster::AttackFinish()
{
	bOnAttack = false;
	if (auto Anim = Animation2DComponent.lock())
	{
		Anim->ChangeAnimation("MonsterIdle");
	}
}

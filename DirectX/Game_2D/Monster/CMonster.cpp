#include "CMonster.h"

#include <Component/CAnimation2DComponent.h>
#include <Component/CColliderBox2D.h>
#include <Component/CColliderLine2D.h>
#include <Component/CColliderSphere2D.h>
#include <Component/CMeshComponent.h>
#include <World/CWorld.h>
#include "../Player/CBullet.h"

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

	Body = CreateComponent<CColliderSphere2D>("Body");
	if (auto Body = this->Body.lock())
	{
		Body->SetCollisionProfile("Monster");
		Body->SetRadius(sqrtf(100.f * 100.f + 100.f * 100.f) * 0.5f);
		Body->SetDrawDebug(true);
		Body->SetInheritScale(false);
	}

	//Line2D = CreateComponent<CColliderLine2D>("Line2D");
	//if (auto Line2D = this->Line2D.lock())
	//{
	//	Line2D->SetCollisionProfile("Monster");
	//	Line2D->SetLineDistance(200.f);
	//	Line2D->SetDrawDebug(true);
	//	Line2D->SetInheritScale(false);
	//}

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
			float Degree = GetWorldPosition().GetViewTargetAngleDegree2D(Target->GetWorldPosition(), EAxis::Y);

			SetWorldRotationZ(Degree);

			if (auto Anim = Animation2DComponent.lock())
			{
				Anim->ChangeAnimation("MonsterAttack");
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
		auto Bullet = World->CreateGameObject<CBullet>("Bullet");
		if (auto BulletObj = Bullet.lock())
		{
			FVector	BulletPos = GetWorldPosition() + GetAxis(EAxis::Y) * 75.f;
			BulletObj->SetCollision("MonsterAttack");
			BulletObj->SetWorldPosition(BulletPos);
			BulletObj->SetWorldRotation(GetWorldRotation());
			BulletObj->SetCollisionTargetName("Player");
			BulletObj->CalcCollisionRadius();

			auto Target = this->FireTarget.lock();

			FVector	TargetPos = Target->GetWorldPosition();

			if (Target)
			{
				FVector	Dir = TargetPos - BulletPos;
				Dir.Normalize();

				BulletObj->SetMoveDirection(Dir);
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

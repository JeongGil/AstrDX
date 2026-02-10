#include "CMonster.h"

#include <Component/CAnimation2DComponent.h>
#include <Component/CColliderBox2D.h>
#include <Component/CColliderLine2D.h>
#include <Component/CColliderSphere2D.h>
#include <Component/CMeshComponent.h>
#include <Component/CWidgetComponent.h>
#include <World/CWorld.h>

#include "../Player/CBullet.h"
#include "../UI/CWorldHUD.h"

bool CMonster::Init()
{
	if (!CGameObject::Init())
	{
		return false;
	}

	MeshComponent = CreateComponent<CMeshComponent>("MonsterMesh");
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

	HUDWidget = CreateComponent<CWidgetComponent>("Widget");
	if (auto Widget = HUDWidget.lock())
	{
		Widget->SetInheritScale(false);
		Widget->SetInheritRotation(false);
		Widget->SetRelativePosition(0.f, 140.f, 0.f);
		Widget->SetRelativeScale(80.f, 40.f);

		auto InWidget = Widget->SetWidget<CWorldHUD>("MonsterHUD").lock();

		InWidget->SetSize(80.f, 40.f);
		InWidget->SetPlayerName(TEXT("Monster"));

		OnHPChanged.push_back(std::bind(&CWorldHUD::SetPlayerHP, InWidget.get(), std::placeholders::_1, std::placeholders::_2));
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

float CMonster::TakeDamage(float Damage)
{
	HP = static_cast<int>(std::clamp(static_cast<float>(HP) - Damage, 0.f, static_cast<float>(MaxHP)));

	for (const auto& Callback : OnHPChanged)
	{
		Callback(static_cast<float>(HP), static_cast<float>(MaxHP));
	}

	return Damage;
}

CMonster* CMonster::Clone()
{
	return new CMonster(*this);
}

void CMonster::AttackNotify()
{
	//if (auto World = this->World.lock())
	//{
	//	auto Bullet = World->CreateGameObject<CBullet>("Bullet");
	//	if (auto BulletObj = Bullet.lock())
	//	{
	//		FVector	BulletPos = GetWorldPosition() + GetAxis(EAxis::Y) * 75.f;
	//		BulletObj->SetCollision("MonsterAttack");
	//		BulletObj->SetWorldPosition(BulletPos);
	//		BulletObj->SetWorldRotation(GetWorldRotation());

	//		auto Target = this->FireTarget.lock();

	//		FVector	TargetPos = Target->GetWorldPosition();

	//		if (Target)
	//		{
	//			FVector	Dir = TargetPos - BulletPos;
	//			Dir.Normalize();

	//			BulletObj->SetMoveDirection(Dir);
	//		}
	//	}
	//}
}

void CMonster::AttackFinish()
{
	bOnAttack = false;
	if (auto Anim = Animation2DComponent.lock())
	{
		Anim->ChangeAnimation("MonsterIdle");
	}
}

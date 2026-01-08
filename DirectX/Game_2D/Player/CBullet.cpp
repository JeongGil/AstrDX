#include "CBullet.h"

#include <CColliderBox2D.h>
#include <Component/CMeshComponent.h>
#include <Component/CProjectileMovementComponent.h>
#include "CBulletEffect.h"
#include "../Component/CStateComponent.h"

void CBullet::SetMoveDirection(const FVector& MoveDirection)
{
	bUseMoveDirection = true;
	this->MoveDirection = MoveDirection;
	if (auto Move = MovementComponent.lock())
	{
		Move->SetMoveDirection(MoveDirection);
	}
}

void CBullet::SetCollision(const std::string& Key)
{
	if (auto Body = this->Body.lock())
	{
		Body->SetCollisionProfile(Key);
	}
}

void CBullet::SetEnableMove(bool bEnable)
{
	bEnableMove = bEnable;

	if (auto Move = MovementComponent.lock())
	{
		Move->SetEnable(bEnable);
	}
}

void CBullet::SetDistance(float Distance)
{
	this->Distance = Distance;
	if (auto Move = MovementComponent.lock())
	{
		Move->SetRange(Distance);
	}
}

void CBullet::SetSpeed(float Speed)
{
	this->Speed = Speed;
	if (auto Move = MovementComponent.lock())
	{
		Move->SetSpeed(Speed);
	}
}

void CBullet::MoveEndFunction()
{
	Destroy();
}

void CBullet::OnCollisionBegin(const FVector& HitPoint, CCollider* Other)
{
	Destroy();

	if (auto World = this->World.lock())
	{
		std::weak_ptr<CBulletEffect> WeakEffect = World->CreateGameObject<CBulletEffect>("BulletEffect");
		if (auto Effect = WeakEffect.lock())
		{
			Effect->SetWorldPosition(HitPoint);
		}
	}
}

void CBullet::OnCollisionEnd(CCollider* Other)
{
}

bool CBullet::Init()
{
	if (!CGameObject::Init())
	{
		return false;
	}

	MeshComponent = CreateComponent<CMeshComponent>("Mesh");
	if (auto Mesh = MeshComponent.lock())
	{
		Mesh->SetShader("MaterialColor2D");
		Mesh->SetMesh("CenterRectColor");
		Mesh->SetRelativeScale(50, 50);
	}

	MovementComponent = CreateComponent<CProjectileMovementComponent>("Movement");
	if (auto Move = MovementComponent.lock())
	{
		Move->SetMoveDirection(GetAxis(EAxis::Y));
		Move->SetRangeFunction<CBullet>(this, &CBullet::MoveEndFunction);
		Move->SetRange(Distance);
		Move->SetSpeed(Speed);
		Move->SetUpdateComponent(MeshComponent);
	}

	Body = CreateComponent<CColliderBox2D>("Body");
	if (auto Body = this->Body.lock())
	{
		Body->SetOnCollisionBegin<CBullet>(this, &CBullet::OnCollisionBegin);
		Body->SetOnCollisionEnd<CBullet>(this, &CBullet::OnCollisionEnd);

		Body->SetCollisionProfile("PlayerAttack");
		Body->SetBoxExtend(50.f, 50.f);
		Body->SetDrawDebug(true);
		Body->SetInheritScale(false);
	}

	return true;
}

void CBullet::Update(const float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CBullet::PostUpdate(const float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);

	//if (auto World = this->World.lock())
	//{
	//	std::list<std::weak_ptr<CGameObject>> ColCandidates;
	//	World->FindObjects(CollisionTargetName, ColCandidates);
	//	for (const auto& WeakOther : ColCandidates)
	//	{
	//		if (auto Other = WeakOther.lock())
	//		{
	//			FVector OtherHalfExtent = Other->GetWorldScale() * 0.5f;
	//			float OtherRadius = sqrtf(OtherHalfExtent.x * OtherHalfExtent.x + OtherHalfExtent.y * OtherHalfExtent.y);
	//			float RadiusSum = OtherRadius + CollisionRadius;
	//
	//			// Collision occurred.
	//			if (Other->GetWorldPosition().SqrDistance(GetWorldPosition()) <= RadiusSum * RadiusSum)
	//			{
	//				std::weak_ptr<CBulletEffect> WeakEffect = World->CreateGameObject<CBulletEffect>("BulletEffect");
	//				if (auto Effect = WeakEffect.lock())
	//				{
	//					Effect->SetWorldPosition(GetWorldPosition());
	//				}
	//
	//				// Process damage event.
	//				auto WeakState = Other->FindComponent<CStateComponent>("State");
	//				if (auto State = WeakState.lock())
	//				{
	//					State->AddHP(-1);
	//				}
	//
	//				Destroy();
	//			}
	//		}
	//	}
	//}
}

CBullet* CBullet::Clone()
{
	return new CBullet(*this);
}

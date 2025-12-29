#include "CBullet.h"

#include "CBulletEffect.h"
#include "Component/CMeshComponent.h"
#include "../Component/CStateComponent.h"

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

	return true;
}

void CBullet::Update(const float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	constexpr float Speed = 300;
	float DelDist = Speed * DeltaTime;

	FVector MoveDirection = bUseMoveDirection ? this->MoveDirection : GetAxis(EAxis::Y);
	FVector DelPos = MoveDirection * DelDist;

	Distance -= DelDist;

	AddWorldPosition(DelPos);

	if (Distance <= 0.f)
	{
		Destroy();
	}

	if (auto World = this->World.lock())
	{
		std::list<std::weak_ptr<CGameObject>> ColCandidates;
		World->FindObjects(CollisionTargetName, ColCandidates);
		for (const auto& WeakOther : ColCandidates)
		{
			if (auto Other = WeakOther.lock())
			{
				FVector OtherHalfExtent = Other->GetWorldScale() * 0.5f;
				float OtherRadius = sqrtf(OtherHalfExtent.x * OtherHalfExtent.x + OtherHalfExtent.y * OtherHalfExtent.y);
				float RadiusSum = OtherRadius + CollisionRadius;

				// Collision occurred.
				if (Other->GetWorldPosition().SqrDistance(GetWorldPosition()) <= RadiusSum * RadiusSum)
				{
					std::weak_ptr<CBulletEffect> WeakEffect = World->CreateGameObject<CBulletEffect>("BulletEffect");
					if (auto Effect = WeakEffect.lock())
					{
						Effect->SetWorldPosition(GetWorldPosition());
					}

					// Process damage event.
					auto WeakState = Other->FindComponent<CStateComponent>("State");
					if (auto State = WeakState.lock())
					{
						State->AddHP(-1);
					}

					Destroy();
				}
			}
		}
	}
}

CBullet* CBullet::Clone()
{
	return new CBullet(*this);
}

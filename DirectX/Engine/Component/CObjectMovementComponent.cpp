#include "CObjectMovementComponent.h"

#include "CSceneComponent.h"
#include "../World/CWorld.h"
#include "../World/CWorldNavigation.h"

bool CObjectMovementComponent::Init()
{
	if (!CMovementComponent::Init())
	{
		return false;
	}

	return true;
}

void CObjectMovementComponent::Update(const float Delta)
{
	CMovementComponent::Update(Delta);
}

void CObjectMovementComponent::PostUpdate(const float DeltaTime)
{
	CMovementComponent::PostUpdate(DeltaTime);

	auto Cmp = UpdateComponent.lock();
	if (!Cmp)
	{
		Velocity = FVector::Zero;
		return;
	}

	if (Paths.empty())
	{
		if (MoveDirection.IsZero())
		{
			Velocity = FVector::Zero;
			return;
		}

		MoveDirection.Normalize();

		Velocity = MoveDirection * (Speed * DeltaTime);
		Cmp->AddWorldPosition(Velocity);
	}
	// Pathfinding
	else
	{
		FVector Target = Paths.at(PathIndex);

		// 이동 방향을 구한다.
		FVector3	Dir = Target - Cmp->GetWorldPosition();
		Dir.z = 0.f;
		Dir.Normalize();

		Velocity = Dir * Speed * DeltaTime;

		Cmp->AddWorldPosition(Velocity);

		MoveDist -= Speed * DeltaTime;

		// 이동된 위치와 타겟까지의 거리를 구한다.
		float Dist = Target.Distance(Cmp->GetWorldPosition());

		// 도착점에 도착했을 경우
		if (Dist <= AcceptDistance || MoveDist < 0.f)
		{
			++PathIndex;

			Cmp->SetWorldPosition(Target);

			// 이동을 종료시킨다.
			if (PathIndex >= Paths.size())
			{
				Paths.clear();
				PathIndex = 0;
				MoveDist = 0.f;
			}

			// 다음 포인트로 이동해야 할 경우
			else
			{
				MoveDist = Paths.at(PathIndex).Distance(Cmp->GetWorldPosition());
			}
		}
	}
}

void CObjectMovementComponent::PostRender()
{
	CMovementComponent::PostRender();

	MoveDirection = FVector::Zero;
}

void CObjectMovementComponent::Destroy()
{
	CMovementComponent::Destroy();
}

void CObjectMovementComponent::Move(const FVector& Pos)
{
	Paths.clear();
	Paths.emplace_back(Pos);
}

void CObjectMovementComponent::Move(const FVector2& Pos)
{
	Move(FVector(Pos));
}

void CObjectMovementComponent::MovePath(const FVector& Pos)
{
	if (auto World = this->World.lock())
	{
		if (auto Nav = World->GetNavigation().lock())
		{
			if (auto UpdateComponent = this->UpdateComponent.lock())
			{
				Nav->FindPath(UpdateComponent->GetWorldPosition(), Pos, this);
			}
		}
	}
}

void CObjectMovementComponent::MovePath(const FVector2& Pos)
{
	MovePath(FVector(Pos));
}

void CObjectMovementComponent::StartPath()
{
	if (auto UpdateComponent = this->UpdateComponent.lock())
	{
		MoveDist = Paths.at(PathIndex).Distance(UpdateComponent->GetWorldPosition());
	}
}

CObjectMovementComponent* CObjectMovementComponent::Clone() const
{
	return new CObjectMovementComponent(*this);
}

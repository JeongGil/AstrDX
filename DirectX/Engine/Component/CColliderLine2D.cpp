#include "CColliderLine2D.h"

#include "CCollision.h"
#include "../Asset/CAssetManager.h"
#include "../Asset/Mesh/CMeshManager.h"
#include "../Asset/Shader/CCBufferCollider.h"
#include "../Asset/Shader/CShaderManager.h"
#include "../World/CWorld.h"

void CColliderLine2D::SetDrawDebug(bool bDrawDebug)
{
	CCollider::SetDrawDebug(bDrawDebug);

	if (bDrawDebug && Shader.expired())
	{
		if (auto ShaderMgr = CAssetManager::GetInst()->GetShaderManager().lock())
		{
			Shader = ShaderMgr->FindShader("Collider");
		}

		if (auto World = this->World.lock())
		{
			if (auto AssetMgr = World->GetWorldAssetManager().lock())
			{
				Mesh = AssetMgr->FindMesh("LineUP2D");
			}
		}
		else
		{
			if (auto MeshMgr = CAssetManager::GetInst()->GetMeshManager().lock())
			{
				Mesh = MeshMgr->FindMesh("Mesh_LineUP2D");
			}
		}

		ColliderCBuffer.reset(new CCBufferCollider);
		ColliderCBuffer->Init();
	}
}

bool CColliderLine2D::Collide(FVector3& OutHitPoint, std::shared_ptr<CCollider> Other)
{
	switch (Other->GetColliderType())
	{
		case EColliderType::Box2D:
			return CCollision::CollideBox2DToLine2D(OutHitPoint, dynamic_cast<CColliderBox2D*>(Other.get()), this);
		case EColliderType::Sphere2D:
			return CCollision::CollideSphere2DToLine2D(OutHitPoint, dynamic_cast<CColliderSphere2D*>(Other.get()), this);
		case EColliderType::Line2D:
			return CCollision::CollideLine2DToLine2D(OutHitPoint, dynamic_cast<CColliderLine2D*>(Other.get()), this);
		default:
			break;
	}

	return false;
}

bool CColliderLine2D::CollideManifold(FCollisionManifold& HitResult, std::shared_ptr<CCollider> Dest)
{
	switch (Dest->GetColliderType())
	{
		case EColliderType::Box2D:
			// If both rotations are 0, perform AABB collision; otherwise, perform OBB collision.
			return CCollision::ManifoldLine2DToBox2D(HitResult, this, dynamic_cast<CColliderBox2D*>(Dest.get()));
		case EColliderType::Sphere2D:
			return CCollision::ManifoldLine2DToSphere2D(HitResult, this, dynamic_cast<CColliderSphere2D*>(Dest.get()));
		case EColliderType::Line2D:
			return CCollision::ManifoldLine2DToLine2D(HitResult, this, dynamic_cast<CColliderLine2D*>(Dest.get()));
	}

	return false;
}

bool CColliderLine2D::CollideMouse(const FVector2& MousePos)
{
	[[maybe_unused]] FVector Hit;
	FVector Point(MousePos.x, MousePos.y, 0.f);

	return CCollision::CollideLine2DToPoint(Hit, Info, Point);
}

bool CColliderLine2D::Init()
{
	if (!CCollider::Init())
	{
		return false;
	}

	if (bDrawDebug)
	{
		if (auto ShaderMgr = CAssetManager::GetInst()->GetShaderManager().lock())
		{
			Shader = ShaderMgr->FindShader("Collider");
		}

		if (auto World = this->World.lock())
		{
			if (auto AssetMgr = World->GetWorldAssetManager().lock())
			{
				Mesh = AssetMgr->FindMesh("LineUP2D");
			}
		}
		else
		{
			if (auto MeshMgr = CAssetManager::GetInst()->GetMeshManager().lock())
			{
				Mesh = MeshMgr->FindMesh("Mesh_LineUP2D");
			}
		}

		ColliderCBuffer.reset(new CCBufferCollider);
		ColliderCBuffer->Init();
	}

	return true;
}

void CColliderLine2D::Update(const float DeltaTime)
{
	CCollider::Update(DeltaTime);
}

void CColliderLine2D::PostUpdate(const float DeltaTime)
{
	CCollider::PostUpdate(DeltaTime);

	UpdateInfo();
}

void CColliderLine2D::UpdateInfo()
{
	Info.Start = WorldPosition;

	FVector Dir = LineDirection.TransformNormal(RotationMatrix).GetNormalized();
	Info.End = Info.Start + Dir * Distance;

	Min.x = std::fminf(Info.Start.x, Info.End.x);
	Min.y = std::fminf(Info.Start.y, Info.End.y);

	Max.x = std::fmaxf(Info.Start.x, Info.End.x);
	Max.y = std::fmaxf(Info.Start.y, Info.End.y);

	RenderScale.x = 1.f;
	RenderScale.y = Distance;
	RenderScale.z = 1.f;
}

CColliderLine2D* CColliderLine2D::Clone() const
{
	return new CColliderLine2D(*this);
}

CColliderLine2D::CColliderLine2D()
{
	ColliderType = EColliderType::Line2D;
}

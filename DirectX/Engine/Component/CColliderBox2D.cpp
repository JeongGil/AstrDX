#include "CColliderBox2D.h"

#include "CCollision.h"
#include "../Asset/CAssetManager.h"
#include "../Asset/Mesh/CMeshManager.h"
#include "../Asset/Shader/CCBufferCollider.h"
#include "../Asset/Shader/CShaderManager.h"
#include "../World/CWorld.h"
#include "CColliderSphere2D.h"

bool CColliderBox2D::Collide(FVector3& OutHitPoint, std::shared_ptr<CCollider> Other)
{
	switch (Other->GetColliderType())
	{
		case EColliderType::Box2D:
			return CCollision::CollideBox2DToBox2D(OutHitPoint, this, dynamic_cast<CColliderBox2D*>(Other.get()));
		case EColliderType::Sphere2D:
			return CCollision::CollideBox2DToSphere2D(OutHitPoint, this, dynamic_cast<CColliderSphere2D*>(Other.get()));
		case EColliderType::Line2D:
			return CCollision::CollideBox2DToLine2D(OutHitPoint, this, dynamic_cast<CColliderLine2D*>(Other.get()));
		default:
			break;
	}

	return false;
}

bool CColliderBox2D::CollideManifold(FCollisionManifold& HitResult, std::shared_ptr<CCollider> Dest)
{
	switch (Dest->GetColliderType())
	{
		case EColliderType::Box2D:
			// If both rotations are 0, perform AABB collision; otherwise, perform OBB collision.
			return CCollision::ManifoldBox2DToBox2D(HitResult, this, dynamic_cast<CColliderBox2D*>(Dest.get()));
		case EColliderType::Sphere2D:
			return CCollision::ManifoldBox2DToSphere2D(HitResult, this, dynamic_cast<CColliderSphere2D*>(Dest.get()));
		case EColliderType::Line2D:
			return CCollision::ManifoldBox2DToLine2D(HitResult, this, dynamic_cast<CColliderLine2D*>(Dest.get()));
	}

	return false;
}

void CColliderBox2D::SetDrawDebug(bool bDrawDebug)
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
				Mesh = AssetMgr->FindMesh("CenterFrameRect");
			}
		}
		else
		{
			if (auto MeshMgr = CAssetManager::GetInst()->GetMeshManager().lock())
			{
				Mesh = MeshMgr->FindMesh("Mesh_CenterFrameRect");
			}
		}

		ColliderCBuffer.reset(new CCBufferCollider);
		ColliderCBuffer->Init();
	}
}

bool CColliderBox2D::Init()
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
				Mesh = AssetMgr->FindMesh("CenterFrameRect");
			}
		}
		else
		{
			if (auto MeshMgr = CAssetManager::GetInst()->GetMeshManager().lock())
			{
				Mesh = MeshMgr->FindMesh("Mesh_CenterFrameRect");
			}
		}
	}

	return true;
}

void CColliderBox2D::Update(const float DeltaTime)
{
	CCollider::Update(DeltaTime);
}

void CColliderBox2D::PostUpdate(const float DeltaTime)
{
	CCollider::PostUpdate(DeltaTime);

	UpdateInfo();
}

void CColliderBox2D::UpdateInfo()
{
	Info.Center = WorldPosition;

	Info.Axis[EAxis::X] = WorldAxis[EAxis::X];
	Info.Axis[EAxis::Y] = WorldAxis[EAxis::Y];

	FVector Pos[4]
	{
		Info.Center - Info.Axis[EAxis::X] * Info.HalfExtent.x - Info.Axis[EAxis::Y] * Info.HalfExtent.y,
		Info.Center - Info.Axis[EAxis::X] * Info.HalfExtent.x + Info.Axis[EAxis::Y] * Info.HalfExtent.y,
		Info.Center + Info.Axis[EAxis::X] * Info.HalfExtent.x - Info.Axis[EAxis::Y] * Info.HalfExtent.y,
		Info.Center + Info.Axis[EAxis::X] * Info.HalfExtent.x + Info.Axis[EAxis::Y] * Info.HalfExtent.y,
	};

	Min = Pos[0];
	Max = Pos[0];

	for (const auto& P : Pos)
	{
		Min.x = min(Min.x, P.x);
		Min.y = min(Min.y, P.y);
		Max.x = max(Max.x, P.x);
		Max.y = max(Max.y, P.y);
	}

	RenderScale.x = WorldScale.x * Info.HalfExtent.x * 2.f;
	RenderScale.y = WorldScale.y * Info.HalfExtent.y * 2.f;
	RenderScale.z = 1.f;
}

CColliderBox2D* CColliderBox2D::Clone() const
{
	return new CColliderBox2D(*this);
}

CColliderBox2D::CColliderBox2D()
{
	ColliderType = EColliderType::Box2D;
}

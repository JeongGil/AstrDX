#include "CColliderSphere2D.h"

#include "CCollision.h"
#include "../Asset/CAssetManager.h"
#include "../Asset/Shader/CShaderManager.h"
#include "../World/CWorld.h"
#include "../Asset/Mesh/CMeshManager.h"
#include "../Asset/Shader/CCBufferCollider.h"

bool CColliderSphere2D::Init()
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
			if (auto World = this->World.lock())
			{
				if (auto AssetMgr = World->GetWorldAssetManager().lock())
				{
					Mesh = AssetMgr->FindMesh("FrameSphere2D");
				}
			}
			else
			{
				if (auto MeshMgr = CAssetManager::GetInst()->GetMeshManager().lock())
				{
					Mesh = MeshMgr->FindMesh("Mesh_FrameSphere2D");
				}
			}
		}

		ColliderCBuffer.reset(new CCBufferCollider);
		ColliderCBuffer->Init();
	}

	return true;
}

void CColliderSphere2D::Update(const float DeltaTime)
{
	CCollider::Update(DeltaTime);
}

void CColliderSphere2D::PostUpdate(const float DeltaTime)
{
	CCollider::PostUpdate(DeltaTime);

	Info.Center = WorldPosition + Offset;

	RenderScale.x = WorldScale.x = Info.Radius;
	RenderScale.y = WorldScale.y = Info.Radius;
	RenderScale.z = 1.f;
}

CColliderSphere2D* CColliderSphere2D::Clone() const
{
	return new CColliderSphere2D(*this);
}

void CColliderSphere2D::SetDrawDebug(bool bDrawDebug)
{
	CCollider::SetDrawDebug(bDrawDebug);

	if (bDrawDebug && Shader.expired())
	{
		if (auto ShaderMgr = CAssetManager::GetInst()->GetShaderManager().lock())
		{
			Shader = ShaderMgr->FindShader("Collider");

			if (auto World = this->World.lock())
			{
				if (auto AssetMgr = World->GetWorldAssetManager().lock())
				{
					Mesh = AssetMgr->FindMesh("FrameSphere2D");
				}
			}
			else
			{
				if (auto MeshMgr = CAssetManager::GetInst()->GetMeshManager().lock())
				{
					Mesh = MeshMgr->FindMesh("Mesh_FrameSphere2D");
				}
			}
		}

		ColliderCBuffer.reset(new CCBufferCollider);
		ColliderCBuffer->Init();
	}
}

bool CColliderSphere2D::Collide(FVector3& OutHitPoint, std::shared_ptr<CCollider> Other)
{
	switch (Other->GetColliderType())
	{
	case EColliderType::Box2D:
		return false;
		//return CCollision::CollideBox2DToBox2D(OutHitPoint, dynamic_cast<CColliderBox2D*>(Other.get()), this);
	case EColliderType::Sphere2D:
		return false;
	case EColliderType::Line2D:
		break;
	default:
		return false;
	}
}

CColliderSphere2D::CColliderSphere2D()
{
	ColliderType = EColliderType::Sphere2D;
}

#include "CColliderLine2D.h"

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
			break;
		case EColliderType::Sphere2D:
			break;
		case EColliderType::Line2D:
			break;
		default:
			break;
	}

	return false;
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

	Info.Start = WorldPosition + Offset;
	Info.End = Info.Start + LineDirection * Distance;

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

#include "CCollisionQuadTree.h"

#include "CWorld.h"
#include "../CDevice.h"
#include "../Utils.h"
#include "../Asset/CAssetManager.h"
#include "../Asset/Mesh/CMesh.h"
#include "../Asset/Mesh/CMeshManager.h"
#include "../Asset/Shader/CCBufferCollider.h"
#include "../Asset/Shader/CShader.h"
#include "../Asset/Shader/CShaderManager.h"

CCollisionQuadTree::CCollisionQuadTree()
{
}

CCollisionQuadTree::~CCollisionQuadTree()
{
}

void CCollisionQuadTree::AddCollisionNode(const std::weak_ptr<CCollisionQuadTreeNode>& Node)
{
	CollisionNodes.push_back(Node);
}

void CCollisionQuadTree::EraseCollisionNode(const std::weak_ptr<CCollisionQuadTreeNode>& Node)
{
	auto Target = Node.lock();

	for (auto It = CollisionNodes.begin(); It != CollisionNodes.end(); ++It)
	{
		if (IsSameTarget(*It, Node))
		{
			CollisionNodes.erase(It);
			break;
		}
	}
}

bool CCollisionQuadTree::Init()
{
#ifdef _DEBUG

	auto ShaderMgr = CAssetManager::GetInst()->GetShaderManager().lock();

	Shader = ShaderMgr->FindShader("Collider");

	if (auto World = this->World.lock())
	{
		auto AssetMgr = World->GetWorldAssetManager().lock();
		Mesh = AssetMgr->FindMesh("CenterFrameRect");
	}
	else
	{
		auto MeshMgr = CAssetManager::GetInst()->GetMeshManager().lock();
		Mesh = MeshMgr->FindMesh("Mesh_CenterFrameRect");
	}

	ColliderCBuffer.reset(new CCBufferCollider);
	ColliderCBuffer->Init();
	ColliderCBuffer->SetColor(FColor::Blue);

#endif

	return true;
}

void CCollisionQuadTree::AddCollider(const std::weak_ptr<CCollider>& Collider)
{
	if (!Root)
	{
		Root.reset(new CCollisionQuadTreeNode);

		Root->SetWorld(World);
		Root->SetTree(std::dynamic_pointer_cast<CCollisionQuadTree>(shared_from_this()));

		auto World = this->World.lock();
		auto CamMgr = World->GetCameraManager().lock();

		Root->Center = CamMgr->GetMainCameraWorldPosition();

		const FResolution& RS = CDevice::GetInst()->GetResolution();

		Root->Size.x = RS.Width * 1.5f;
		Root->Size.y = RS.Height* 1.5f;
	}

	Root->AddCollider(Collider, NodePool);
}

void CCollisionQuadTree::Update(const float DeltaTime)
{
	if (Root)
	{
		auto World = this->World.lock();
		auto CamMgr = World->GetCameraManager().lock();

		Root->Center = CamMgr->GetMainCameraWorldPosition();
	}
}

void CCollisionQuadTree::Collide(const float DeltaTime)
{
	for (const auto& WNode : CollisionNodes)
	{
		if (auto Node = WNode.lock())
		{
			Node->Collide(DeltaTime);
		}
	}

	CollisionNodes.clear();
}

void CCollisionQuadTree::ReturnNodePool()
{
	if (Root)
	{
		Root->ReturnNodePool(NodePool);
	}
}

void CCollisionQuadTree::Render()
{
#ifdef _DEBUG
	
	ColliderCBuffer->UpdateBuffer();
	
	if (Root)
	{
		Root->Render(Mesh, Shader);
	}

#endif

}

void CCollisionQuadTree::UpdateInfo()
{
}

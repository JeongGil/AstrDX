#include "CCollisionQuadTreeNode.h"

#include "CCollisionQuadTree.h"
#include "CWorld.h"
#include "../Asset/Mesh/CMesh.h"
#include "../Asset/Shader/CCBufferTransform.h"
#include "../Asset/Shader/CShader.h"
#include "../Component/CCollider.h"
#include "../Component/CCollision.h"
#include "../Object/CGameObject.h"

CCollisionQuadTreeNode::CCollisionQuadTreeNode()
{
#ifdef _DEBUG

	TransformCBuffer.reset(new CCBufferTransform);
	TransformCBuffer->Init();

#endif
}

CCollisionQuadTreeNode::~CCollisionQuadTreeNode()
{
}

void CCollisionQuadTreeNode::AddCollider(const std::weak_ptr<CCollider>& Collider,
	std::vector<std::shared_ptr<CCollisionQuadTreeNode>>& NodePool)
{
	if (!IsInCollider(Collider))
	{
		return;
	}

	if (!Children[0])
	{
		Colliders.push_back(Collider);

		auto Tree = this->Tree.lock();

		if (Colliders.size() == 2)
		{
			Tree->AddCollisionNode(std::dynamic_pointer_cast<CCollisionQuadTreeNode>(shared_from_this()));
		}

		if (Colliders.size() >= QUADTREE_DIVISION_COUNT && Depth < QUADTREE_DEPTH_MAX)
		{
			CreateChild(NodePool);

			for (const auto& Col : Colliders)
			{
				for (const auto& Child : Children)
				{
					if (Child->IsInCollider(Col))
					{
						Child->AddCollider(Col, NodePool);
					}
				}
			}

			Colliders.clear();

			Tree->EraseCollisionNode(std::dynamic_pointer_cast<CCollisionQuadTreeNode>(shared_from_this()));
		}
	}
	else
	{
		for (const auto& Child : Children)
		{
			Child->AddCollider(Collider, NodePool);
		}
	}
}

void CCollisionQuadTreeNode::Collide(const float DeltaTime)
{
	auto SrcIt = Colliders.begin();
	while (SrcIt != Colliders.end())
	{
		if (SrcIt->expired())
		{
			SrcIt = Colliders.erase(SrcIt);
			continue;
		}

		auto SrcCollider = SrcIt->lock();

		if (!SrcCollider->GetEnable())
		{
			SrcIt = Colliders.erase(SrcIt);
			continue;
		}

		if (!SrcCollider->GetEnable())
		{
			++SrcIt;
			continue;
		}

		auto SrcProfile = SrcCollider->GetCollisionProfile();

		if (!SrcProfile->bEnable)
		{
			++SrcIt;
			continue;
		}

		auto DestIt = SrcIt;
		++DestIt;

		while (DestIt != Colliders.end())
		{
			if (DestIt->expired())
			{
				DestIt = Colliders.erase(DestIt);
				continue;
			}

			auto DestCollider = DestIt->lock();

			if (!DestCollider->GetEnable())
			{
				DestIt = Colliders.erase(DestIt);
				continue;
			}

			if (!DestCollider->GetEnable())
			{
				++DestIt;
				continue;
			}

			auto DestProfile = DestCollider->GetCollisionProfile();

			if (!DestProfile->bEnable)
			{
				++DestIt;
				continue;
			}

			auto SrcToDest = SrcProfile->Interaction[DestProfile->Channel->Channel];
			auto DestToSrc = DestProfile->Interaction[SrcProfile->Channel->Channel];

			if (SrcToDest == ECollisionInteraction::Ignore || DestToSrc == ECollisionInteraction::Ignore)
			{
				++DestIt;
				continue;
			}

			if (SrcToDest != DestToSrc)
			{
				++DestIt;
				continue;
			}

			FVector HitPoint, BlockMove;
			if (SrcCollider->Collide(HitPoint, DestCollider))
			{
				if (SrcToDest == ECollisionInteraction::Block)
				{
					auto SrcObj = SrcCollider->GetOwner().lock();
					auto DestObj = DestCollider->GetOwner().lock();

					FVector	SrcVelocity = SrcObj->GetVelocity();
					FVector	DestVelocity = DestObj->GetVelocity();

					FVector	SrcPos = SrcObj->GetWorldPosition();
					FVector	DestPos = DestObj->GetWorldPosition();

					FCollisionManifold	Manifold;

					if (SrcCollider->CollideManifold(Manifold, DestCollider))
					{
						float SrcInvMass = 0.f;
						float DestInvMass = 0.f;

						if (!SrcCollider->GetStatic())
							SrcInvMass = 1.f;

						if (!DestCollider->GetStatic())
							DestInvMass = 1.f;

						if (CCollision::ResolveSlideStop2D(SrcPos,
							SrcVelocity, SrcInvMass,
							DestPos, DestVelocity,
							DestInvMass, Manifold))
						{
							SrcObj->ClearPhysics();
							DestObj->ClearPhysics();
						}

						SrcObj->SetWorldPosition(SrcPos);
						DestObj->SetWorldPosition(DestPos);
					}

					HitPoint = Manifold.ContactPoint;

					SrcCollider->CallOnCollisionBlock(HitPoint, *DestIt);

					DestCollider->CallOnCollisionBlock(HitPoint, *SrcIt);
				}
				else
				{
					if (!SrcCollider->CheckCollidingObject(DestCollider.get()))
					{
						SrcCollider->CallOnCollisionBegin(HitPoint, *DestIt);

						DestCollider->CallOnCollisionBegin(HitPoint, *SrcIt);
					}
				}
			}

			// If two objects did not collide in the current frame but were colliding until the previous frame,
			// it indicates that they are just starting to separate.
			else if (SrcCollider->CheckCollidingObject(DestCollider.get()))
			{
				if (SrcToDest == ECollisionInteraction::Overlap)
				{
					SrcCollider->CallOnCollisionEnd(DestCollider.get());
					DestCollider->CallOnCollisionEnd(SrcCollider.get());
				}

				else
				{
				}
			}

			++DestIt;
		}

		++SrcIt;
	}
}

void CCollisionQuadTreeNode::Render(const std::weak_ptr<CMesh>& Mesh, const std::weak_ptr<CShader>& Shader)
{
	FMatrix ViewMat;
	FMatrix ProjMat;

	if (auto World = this->World.lock())
	{
		if (auto CamMgr = World->GetCameraManager().lock())
		{
			ViewMat = CamMgr->GetViewMatrix();
			ProjMat = CamMgr->GetProjectionMatrix();
		}
	}

	auto RenderMesh = Mesh.lock();

	FMatrix ScaleMatrix;
	ScaleMatrix.Scaling(Size);

	FMatrix TranslateMatrix;
	TranslateMatrix.Translation(Center);

	FMatrix WorldMatrix = ScaleMatrix * TranslateMatrix;

	TransformCBuffer->SetWorldMatrix(WorldMatrix);
	TransformCBuffer->SetViewMatrix(ViewMat);
	TransformCBuffer->SetProjectionMatrix(ProjMat);

	TransformCBuffer->UpdateBuffer();

	auto RenderShader = Shader.lock();

	RenderShader->SetShader();

	RenderMesh->Render();

	if (Children[0])
	{
		for (const auto& Child : Children)
		{
			Child->Render(Mesh, Shader);
		}
	}
}

void CCollisionQuadTreeNode::ReturnNodePool(std::vector<std::shared_ptr<CCollisionQuadTreeNode>>& NodePool)
{
	Colliders.clear();

	if (Children[0])
	{
		for (auto& Child : Children)
		{
			NodePool.push_back(Child);

			Child->ReturnNodePool(NodePool);

			Child.reset();
		}
	}
}

bool CCollisionQuadTreeNode::IsInCollider(const std::weak_ptr<CCollider>& Collider)
{
	auto Src = Collider.lock();

	FVector Min = Src->GetMin();
	FVector Max = Src->GetMax();

	FVector NodeMin = Center - Size * 0.5f;
	FVector NodeMax = NodeMin + Size;

	if (Min.x > NodeMax.x)
	{
		return false;
	}
	else if (Max.x < NodeMin.x)
	{
		return false;
	}
	else if (Min.y > NodeMax.y)
	{
		return false;
	}
	else if (Max.y < NodeMin.y)
	{
		return false;
	}

	return true;
}

void CCollisionQuadTreeNode::CreateChild(std::vector<std::shared_ptr<CCollisionQuadTreeNode>>& NodePool)
{
	if (NodePool.empty())
	{
		NodePool.resize(4);

		for (auto& Node : NodePool)
		{
			Node.reset(new CCollisionQuadTreeNode);
		}
	}

	for (int i = 0; i < 4; i++)
	{
		auto& Child = Children[i];
		Child = NodePool.back();

		NodePool.pop_back();

		Child->SetWorld(World);
		Child->Tree = Tree;
		Child->Parent = std::dynamic_pointer_cast<CCollisionQuadTreeNode>(shared_from_this());
		Child->Depth = Depth + 1;

		Child->Size = Size * 0.5f;

		if (i % 2 == 0)
		{
			Child->Center.x = Center.x - Size.x * 0.25f;
		}
		else
		{
			Child->Center.x = Center.x + Size.x * 0.25f;
		}

		if (i <= 1)
		{
			Child->Center.y = Center.y + Size.y * 0.25f;
		}
		else
		{
			Child->Center.y = Center.y - Size.y * 0.25f;
		}
	}
}

#include "CWeapon_Battle.h"

#include <Component/CColliderBox2D.h>
#include <Component/CMeshComponent.h>
#include <World/CWorld.h>
#include <Asset/Material/CMaterial.h>

#include "CNonPlayerCharacter.h"
#include "../Strings.h"
#include "../../Engine/CTimer.h"
#include "../Table/WeaponTable.h"


bool CWeapon_Battle::Init()
{
	if (!CGameObject::Init())
	{
		return false;
	}

	Root = CreateComponent<CSceneComponent>(Key::Comp::Root);
	
	Collider = CreateComponent<CColliderBox2D>(Key::Comp::Collider, Key::Comp::Root);
	if (auto Collider = this->Collider.lock())
	{
		Collider->SetRelativeRotationZ(90);
		Collider->SetDrawDebug(true);

		Collider->SetEnable(false);
		Collider->SetCollisionProfile("PlayerAttack");
	}

	Mesh = CreateComponent<CMeshComponent>(Key::Comp::Mesh, Key::Comp::Root);
	if (auto Mesh = this->Mesh.lock())
	{
		Mesh->SetRelativeRotationZ(90);

		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("CenterRectTex");

		Mesh->SetBlendState(0, "AlphaBlend");

		Mesh->SetInheritScale(true);
		Mesh->SetInheritRotation(true);

		Mesh->TrySetRenderLayer(ERenderOrder::CharacterWeapon);
	}

	return true;
}

void CWeapon_Battle::Update(const float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	if (auto Anchor = PosAnchor.lock())
	{
		SetWorldPosition(Anchor->GetWorldPosition());

//#ifdef _DEBUG
//
//		char Test[256] = {};
//		auto Pos = Mesh.lock()->GetWorldPosition(); //Anchor->GetWorldPosition() - GetWorldPosition();
//		sprintf_s(Test, "Weapon: %p, WeaponPos : %2f, %2f, %2f\n", (void*)Mesh.lock().get(), Pos.x, Pos.y, Pos.z);
//
//		OutputDebugStringA(Test);
//
//#endif
	}

	//Target = GetClosestEnemy();
	//if (auto Target = this->Target.lock())
	//{
	//	// Look at target.
	//	float Degree = GetWorldPosition().GetViewTargetAngleDegree2D(Target->GetWorldRotation(), EAxis::Y);
	//	SetWorldRotationZ(Degree);

	//	// Attack
	//	if (auto InvenWeapon = Origin.lock())
	//	{
	//		FWeaponInfo* Info;
	//		if (WeaponTable::GetInst().TryGet(InvenWeapon->GetWeaponInfoID(), Info))
	//		{
	//			using Clock = std::chrono::steady_clock;

	//			auto Now = CTimer::Now();
	//			auto Elapsed = Now - LastFiredTime;
	//			auto Cooldown = std::chrono::duration_cast<Clock::duration>(std::chrono::duration<float>(Info->CooldownMS * 0.001f));

	//			if (Elapsed >= Cooldown)
	//			{
	//				if ((GetWorldPosition() - Target->GetWorldPosition()).SqrLength() <= Info->Range * Info->Range)
	//				{
	//					// Fire
	//					auto Overflow = Elapsed - Cooldown;
	//					LastFiredTime = Now - Overflow;


	//				}
	//			}
	//		}
	//	}
	//}

	//if (auto InvenWeapon = Origin.lock())
	//{
	//	FWeaponInfo* Info;
	//	if (WeaponTable::GetInst().TryGet(InvenWeapon->GetWeaponInfoID(), Info))
	//	{
	//		//float RTT = Info.ran
	//	}
	//}
}

CWeapon_Battle* CWeapon_Battle::Clone()
{
	return new CWeapon_Battle(*this);
}

void CWeapon_Battle::InitWeaponInfo(TableID ID)
{
	FWeaponInfo* Info;
	if (!WeaponTable::GetInst().TryGet(ID, Info))
	{
		return;
	}

	if (auto Mesh = this->Mesh.lock())
	{
		auto WPath = std::wstring(Info->SpritePath.begin(), Info->SpritePath.end());
		auto WMatTexInfo = Mesh->AddTexture(0, Info->SpritePath, WPath.c_str(), Key::Path::Brotato);
		if (auto MatTexInfo = WMatTexInfo.lock())
		{
			if (auto Texture = MatTexInfo->Texture.lock())
			{
				const FTextureInfo* TexInfo = Texture->GetTexture();
				Mesh->SetWorldScale(TexInfo->Width, TexInfo->Height);

				if (auto Col = Collider.lock())
				{
					Col->SetBoxExtent(TexInfo->Width, TexInfo->Height);
				}
			}
		}
	}
}

std::weak_ptr<CSceneComponent> CWeapon_Battle::GetClosestEnemy()
{
	if (auto World = this->World.lock())
	{
		auto NPCs = World->FindObjectsOfType<CNonPlayerCharacter>();

		float SqrDist = FLT_MAX;
		std::weak_ptr<CNonPlayerCharacter> Closest;
		for (const auto& WNPC : NPCs)
		{
			if (auto NPC = WNPC.lock())
			{
				float CurrSqrDist = (GetWorldPosition() - NPC->GetWorldPosition()).SqrLength();
				if (CurrSqrDist < SqrDist)
				{
					SqrDist = CurrSqrDist;
					Closest = WNPC;
				}
			}
		}

		if (auto Target = Closest.lock())
		{
			return Target->GetRootComponent();
		}
	}

	return std::weak_ptr<CSceneComponent>();
}

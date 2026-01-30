#include "CPlayer.h"

#include <Component/CColliderBox2D.h>
#include <World/CWorld.h>
#include <CDevice.h>
#include <Component/CAnimation2DComponent.h>
#include <Component/CCameraComponent.h>
#include <Component/CColliderLine2D.h>
#include <Component/CColliderSphere2D.h>
#include <Component/CMeshComponent.h>
#include <Component/CObjectMovementComponent.h>
#include <CTimer.h>
#include <Component/CWidgetComponent.h>
#include <World/CWorldUIManager.h>

#include "CBullet.h"
#include "../Component/CStateComponent.h"
#include "../UI/CMainWidget.h"
#include "../UI/CPlayerStateWidget.h"
#include "../UI/CWorldHUD.h"


void CPlayer::TestNotify()
{
	OutputDebugString(TEXT("Test Notify\n"));
}

void CPlayer::AttackNotify()
{
	if (auto World = this->World.lock())
	{
		auto WeakBullet = World->CreateGameObject<CBullet>("Bullet");
		if (auto Bullet = WeakBullet.lock())
		{
			Bullet->SetCollision("PlayerAttack");
			Bullet->SetWorldPosition(GetWorldPosition() + GetAxis(EAxis::Y) * 75.f);
			Bullet->SetWorldRotation(GetWorldRotation());
			Bullet->SetCollisionTargetName("Monster");
			Bullet->CalcCollisionRadius();
		}
	}
}

void CPlayer::AttackFinish()
{
	bAutoIdle = true;

	//if (auto Anim = Animation2DComponent.lock())
	//{
	//	Anim->ChangeAnimation("PlayerIdle");
	//}
}

void CPlayer::MoveUp()
{
	bAutoIdle = true;

	auto Move = MovementComponent.lock();
	auto Mesh = MeshComponent.lock();
	auto Anim = Animation2DComponent.lock();

	Move->AddMove(Mesh->GetAxis(EAxis::Y));
	Anim->ChangeAnimation("PlayerWalk");
}

void CPlayer::MoveDown()
{
	bAutoIdle = true;

	auto Move = MovementComponent.lock();
	auto Mesh = MeshComponent.lock();
	auto Anim = Animation2DComponent.lock();

	Move->AddMove(-Mesh->GetAxis(EAxis::Y));
	Anim->ChangeAnimation("PlayerWalk");
}

void CPlayer::MoveLeft()
{
	bAutoIdle = true;

	auto Move = MovementComponent.lock();
	auto Mesh = MeshComponent.lock();
	auto Anim = Animation2DComponent.lock();

	Move->AddMove(-Mesh->GetAxis(EAxis::X));
	Anim->ChangeAnimation("PlayerWalk");
}

void CPlayer::MoveRight()
{
	bAutoIdle = true;

	auto Move = MovementComponent.lock();
	auto Mesh = MeshComponent.lock();
	auto Anim = Animation2DComponent.lock();

	Move->AddMove(Mesh->GetAxis(EAxis::X));
	Anim->ChangeAnimation("PlayerWalk");
}

void CPlayer::AttackKey()
{
	bAutoIdle = false;

	if (auto Anim = Animation2DComponent.lock())
	{
		Anim->ChangeAnimation("PlayerAttack");
	}
}

void CPlayer::JumpKey()
{
	if (auto Mesh = MeshComponent.lock())
	{
		Mesh->SetSimulatePhysics(true);
		Mesh->SetUseGravity(true);
	}
}

void CPlayer::Skill1Press()
{
	bAutoIdle = true;

	if (auto World = this->World.lock())
	{
		auto WeakBullet = World->CreateGameObject<CBullet>("Bullet");
		if (auto Bullet = WeakBullet.lock())
		{
			Bullet->SetWorldPosition(GetWorldPosition() + GetAxis(EAxis::Y) * 75.f);
			Bullet->SetWorldRotation(GetWorldRotation());
			Bullet->SetCollisionTargetName("Monster");
			Bullet->CalcCollisionRadius();
			Bullet->SetEnableMove(false);
		}
	}
}

void CPlayer::Skill1Hold()
{
	bAutoIdle = false;
	if (auto Bullet = Skill1Bullet.lock())
	{
		Bullet->AddRelativeScale(50.f * CTimer::GetDeltaTime(), 50.f * CTimer::GetDeltaTime());
	}
}

void CPlayer::Skill1Release()
{
	bAutoIdle = true;
	if (auto Bullet = Skill1Bullet.lock())
	{
		Bullet->SetEnableMove(true);
	}
}

bool CPlayer::Init()
{
	if (!CGameObject::Init())
	{
		return false;
	}

	auto World = this->World.lock();

	MeshComponent = CreateComponent<CMeshComponent>("Mesh");
	if (auto Mesh = MeshComponent.lock())
	{
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("RectTex");
		Mesh->SetWorldScale(100, 100);

		Mesh->SetMaterialBaseColor(0, FColor(1, 0, 0, 0));
		Mesh->SetBlendState(0, "AlphaBlend");
		Mesh->SetPivot(0.5f, 0.f);
	}

	Body = CreateComponent<CColliderBox2D>("Body");
	if (auto Body = this->Body.lock())
	{
		Body->SetCollisionProfile("Player");
		Body->SetBoxExtent(100.f, 100.f);
		Body->SetDrawDebug(true);
		Body->SetInheritScale(false);
		Body->SetRelativePosition(0.f, 50.f, 0.f);
	}

	//Sphere2D = CreateComponent<CColliderSphere2D>("Sphere2D");
	//if (auto Sphere2D = this->Sphere2D.lock())
	//{
	//	Sphere2D->SetCollisionProfile("Player");
	//	Sphere2D->SetRadius(sqrtf(20000.f) * 0.5f);
	//	Sphere2D->SetDrawDebug(true);
	//	Sphere2D->SetInheritScale(false);
	//	Sphere2D->SetRelativePosition(0.f, 50.f, 0.f);
	//}

	//Line2D = CreateComponent<CColliderLine2D>("Line2D");
	//if (auto Line2D = this->Line2D.lock())
	//{
	//	Line2D->SetCollisionProfile("Player");
	//	//Line2D->SetRadius(sqrtf(20000.f) * 0.5f);
	//	Line2D->SetLineDistance(200.f);
	//	Line2D->SetDrawDebug(true);
	//	Line2D->SetInheritScale(false);
	//	Line2D->SetRelativePosition(0.f, 100.f, 0.f);
	//}

	Rotation = CreateComponent<CSceneComponent>("Rotation");
	if (auto RotCmp = Rotation.lock())
	{
		RotCmp->SetInheritRotation(false);
		RotCmp->SetInheritScale(false);
	}

	SubMeshComponent = CreateComponent<CMeshComponent>("SubMesh", "Rotation");
	if (auto Mesh = SubMeshComponent.lock())
	{
		Mesh->SetShader("MaterialColor2D");
		Mesh->SetMesh("CenterRectColor");

		Mesh->SetInheritScale(false);
		Mesh->SetRelativePosition(100, 0, 0);
		Mesh->SetRelativeScale(50, 50);

		Mesh->SetMaterialBaseColor(0, FColor::Red);
	}

	CameraComponent = CreateComponent<CCameraComponent>("PlayerCamera");
	if (auto Cam = CameraComponent.lock())
	{
		const auto& Resolution = CDevice::GetInst()->GetResolution();
		Cam->SetProjection(CCameraComponent::EProjectionType::Orthogonal,
			90.f, static_cast<float>(Resolution.Width), static_cast<float>(Resolution.Height), 1000);

		Cam->SetInheritRotation(false);
	}

	HUDWidget = CreateComponent<CWidgetComponent>("Widget");
	if (auto Widget = HUDWidget.lock())
	{
		Widget->SetInheritScale(false);
		Widget->SetInheritRotation(false);
		Widget->SetRelativePosition(0.f, 140.f, 0.f);
		Widget->SetRelativeScale(80.f, 40.f);

		auto InWidget = Widget->SetWidget<CWorldHUD>("PlayerHUD").lock();

		InWidget->SetSize(80.f, 40.f);
		InWidget->SetPlayerName(TEXT("Player"));

		OnHPChanged.push_back(std::bind(&CWorldHUD::SetPlayerHP, InWidget.get(), std::placeholders::_1, std::placeholders::_2));
	}

	StateComponent = CreateComponent<CStateComponent>("State");

	Animation2DComponent = CreateComponent<CAnimation2DComponent>("Animation2D");
	if (auto Anim = Animation2DComponent.lock())
	{
		Anim->SetUpdateComponent(MeshComponent);

		Anim->AddAnimation("PlayerIdle");
		Anim->AddAnimation("PlayerWalk");
		Anim->AddAnimation("PlayerAttack", 0.5f);
		//Anim->SetPlayRate("PlayerAttack", 2.f);
		//Anim->ChangeAnimation("PlayerWalk");

		Anim->AddNotify<CPlayer>("PlayerAttack", "AttackNotify", 2, this, &CPlayer::AttackNotify);
		Anim->SetFinishNotify<CPlayer>("PlayerAttack", this, &CPlayer::AttackFinish);

		Anim->SetLoop("PlayerIdle", true);
		Anim->SetLoop("PlayerWalk", true);
	}

	if (auto Input = World->GetInput().lock())
	{
		Input->AddBindKey("MoveUp", 'W');
		Input->SetBindFunction<CPlayer>("MoveUp", EInputType::Hold, this, &CPlayer::MoveUp);
		//Input->SetKeyCtrl("MoveUp", true);

		Input->AddBindKey("MoveDown", 'S');
		Input->SetBindFunction<CPlayer>("MoveDown", EInputType::Hold, this, &CPlayer::MoveDown);

		Input->AddBindKey("MoveLeft", 'A');
		Input->SetBindFunction<CPlayer>("MoveLeft", EInputType::Hold, this, &CPlayer::MoveLeft);

		Input->AddBindKey("MoveRight", 'D');
		Input->SetBindFunction<CPlayer>("MoveRight", EInputType::Hold, this, &CPlayer::MoveRight);

		Input->AddBindKey("Skill1", VK_LBUTTON);
		Input->SetKeyAlt("Skill1", true);
		Input->SetKeyCtrl("Skill1", true);

		Input->SetBindFunction<CPlayer>("Skill1", EInputType::Press, this, &CPlayer::Skill1Press);
		Input->SetBindFunction<CPlayer>("Skill1", EInputType::Hold, this, &CPlayer::Skill1Hold);
		Input->SetBindFunction<CPlayer>("Skill1", EInputType::Release, this, &CPlayer::Skill1Release);

		Input->AddBindKey("Attack", VK_SPACE);
		Input->SetBindFunction<CPlayer>("Attack", EInputType::Press, this, &CPlayer::AttackKey);

		Input->AddBindKey("Jump", VK_RETURN);
		Input->SetBindFunction<CPlayer>("Jump", EInputType::Press, this, &CPlayer::JumpKey);
	}

	MovementComponent = CreateComponent<CObjectMovementComponent>("Movement");
	if (auto Move = MovementComponent.lock())
	{
		Move->SetUpdateComponent(MeshComponent);
	}

	auto UIManager = World->GetUIManager().lock();
	if (auto MainWidget = UIManager->FindWidget<CMainWidget>("MainWidget").lock())
	{
		if (auto PlayerState = MainWidget->FindWidget<CPlayerStateWidget>("PlayerState").lock())
		{
			PlayerState->SetPlayerName(TEXT("Player"));

			OnHPChanged.push_back(std::bind(&CPlayerStateWidget::SetPlayerHP, PlayerState.get(),
				std::placeholders::_1, std::placeholders::_2));

			OnMPChanged.push_back(std::bind(&CPlayerStateWidget::SetPlayerMP, PlayerState.get(),
				std::placeholders::_1, std::placeholders::_2));
		}
	}

	return true;
}

void CPlayer::Update(const float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	auto Mesh = MeshComponent.lock();
	auto Anim = Animation2DComponent.lock();

	if (Mesh->GetSpeed() == 0.f && bAutoIdle)
	{
		bAutoIdle = false;
		Anim->ChangeAnimation("PlayerIdle");
	}

	//if (auto RotCmp = Rotation.lock())
	//{
	//	RotCmp->AddRelativeRotationZ(100.f * DeltaTime);
	//}

	//if (auto SubMesh = SubMeshComponent.lock())
	//{
	//	static float Elapsed = 0.f;
	//	Elapsed += DeltaTime;
	//	if (Elapsed >= 1.f)
	//	{
	//		Elapsed -= 1.f;
	//		auto& MT = CEngine::GetInst()->GetMT();
	//		std::uniform_real_distribution<float> Dist(0.f, 1.f);
	//		SubMesh->SetMaterialBaseColor(0, Dist(MT), Dist(MT), Dist(MT), 0.f);
	//	}
	//	//SubMesh->AddRelativePosition(FVector(0.2f, 0, 0) * DeltaTime);
	//}

	//if (auto Mesh = MeshComponent.lock())
	//{
	//	auto Anim = Animation2DComponent.lock();

	//	bool bMove = false;

	//	if (GetAsyncKeyState('W') & 0x8000)
	//	{
	//		Mesh->AddRelativePosition(Mesh->GetAxis(EAxis::Y) * 100 * DeltaTime);
	//		Anim->ChangeAnimation("PlayerWalk");
	//		bMove = true;
	//	}

	//	if (GetAsyncKeyState('S') & 0x8000)
	//	{
	//		Mesh->AddRelativePosition(Mesh->GetAxis(EAxis::Y) * -100 * DeltaTime);
	//		Anim->ChangeAnimation("PlayerWalk");
	//		bMove = true;
	//	}

	//	if (GetAsyncKeyState('A') & 0x8000)
	//	{
	//		Mesh->AddRelativeRotationZ(180 * DeltaTime);
	//	}

	//	if (GetAsyncKeyState('D') & 0x8000)
	//	{
	//		Mesh->AddRelativeRotationZ(-180 * DeltaTime);
	//	}

	//	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	//	{
	//		Anim->ChangeAnimation("PlayerAttack");
	//		bOnAttack = true;
	//	}

	//	if (!bMove && !bOnAttack)
	//	{
	//		Anim->ChangeAnimation("PlayerIdle");
	//	}

	//	if (GetAsyncKeyState('1') & 0x8000)
	//	{
	//		if (auto World = this->World.lock())
	//		{
	//			//const std::string BulletName = "Missile_";
	//			//static int Counter = 0;
	//			auto WeakMissile = World->CreateGameObject<CMissile>("Missile");
	//			if (auto Missile = WeakMissile.lock())
	//			{
	//				Missile->SetWorldPosition(GetWorldPosition() + GetAxis(EAxis::Y) * 100);
	//				Missile->SetWorldRotation(GetWorldRotation());
	//				Missile->SetCamera();
	//			}
	//		}
	//	}

	//	if (GetAsyncKeyState('3') & 0x8000)
	//	{
	//		for (int i = 0; i < 12; i++)
	//		{
	//			if (auto World = this->World.lock())
	//			{
	//				auto WeakBullet = World->CreateGameObject<CBullet>("Bullet");
	//				if (auto Bullet = WeakBullet.lock())
	//				{
	//					FMatrix DirMat;
	//					DirMat.RotationZ(static_cast<float>(i) * 30.f);

	//					FVector BulletDir = GetAxis(EAxis::Y).TransformNormal(DirMat).GetNormalized();
	//					constexpr float Offset = 90.f;
	//					FVector BulletPos = GetWorldPosition() + BulletDir * Offset;

	//					Bullet->SetWorldPosition(BulletPos);
	//					Bullet->CalcCollisionRadius();

	//					Bullet->SetCloseTarget<CMonster>();
	//					Bullet->SetCollisionTargetName("Monster");
	//				}
	//			}
	//		}
	//	}
	//}
}

void CPlayer::Destroy()
{
	CGameObject::Destroy();
}

float CPlayer::TakeDamage(float Damage)
{
	HP = std::clamp(static_cast<float>(HP) - Damage, 0.f, static_cast<float>(MaxHP));

	for (const auto& Callback : OnHPChanged)
	{
		Callback(static_cast<float>(HP), static_cast<float>(MaxHP));
	}

	return Damage;
}

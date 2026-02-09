#include "CAnimation2DComponent.h"

#include "CMeshComponent.h"
#include "../Asset/CAssetManager.h"
#include "../Asset/Animation2D/CAnimation2DManager.h"
#include "../Asset/Shader/CCBufferAnimation2D.h"
#include "../Asset/Texture/CTexture.h"
#include "../World/CWorld.h"

CAnimation2DComponent::CAnimation2DComponent(const CAnimation2DComponent& other) :
	CObjectComponent(other),
	UpdateComponent(other.UpdateComponent),
	Animations(other.Animations),
	CurrentAnimation(other.CurrentAnimation)
{
}

CAnimation2DComponent::CAnimation2DComponent(CAnimation2DComponent&& other) noexcept :
	CObjectComponent(std::move(other)),
	UpdateComponent(std::move(other.UpdateComponent)),
	Animations(std::move(other.Animations)),
	CurrentAnimation(std::move(other.CurrentAnimation))
{
}

bool CAnimation2DComponent::Init()
{
	if (!CObjectComponent::Init())
	{
		return false;
	}

	AnimationCBuffer.reset(new CCBufferAnimation2D);

	AnimationCBuffer->Init();
	AnimationCBuffer->SetEnableAnimation2D(true);

	return true;
}

void CAnimation2DComponent::Update(const float DeltaTime)
{
	CObjectComponent::Update(DeltaTime);

	if (CurrentAnimation)
	{
		auto MeshComponent = UpdateComponent.lock();
		if (MeshComponent)
		{
			MeshComponent->SetAnimationComponent(std::dynamic_pointer_cast<CAnimation2DComponent>(shared_from_this()));

			if (!bUpdateEnable)
			{
				if (auto Asset = CurrentAnimation->GetAsset().lock())
				{
					AnimationCBuffer->SetTextureType(Asset->GetTextureType());

					if (MeshComponent->SetTexture(0, 0, Asset->GetTexture()))
					{
						bUpdateEnable = true;
					}
				}
			}
		}

		CurrentAnimation->Update(DeltaTime);
	}
}

void CAnimation2DComponent::PostUpdate(const float Delta)
{
	CObjectComponent::PostUpdate(Delta);
}

void CAnimation2DComponent::Destroy()
{
	CObjectComponent::Destroy();
}

void CAnimation2DComponent::SetUpdateComponent(const std::weak_ptr<CMeshComponent>& Component)
{
	this->UpdateComponent = Component;

	if (auto MeshComponent = UpdateComponent.lock();
		MeshComponent && CurrentAnimation)
	{
		if (auto Asset = CurrentAnimation->GetAsset().lock())
		{
			AnimationCBuffer->SetTextureType(Asset->GetTextureType());

			if (MeshComponent->SetTexture(0, 0, Asset->GetTexture()))
			{
				bUpdateEnable = true;
			}
		}
	}
}

void CAnimation2DComponent::AddAnimation(const std::weak_ptr<CAnimation2D>& Animation, float PlayTime, float PlayRate,
	bool bLoop, bool bReverse)
{
	auto Anim = Animation.lock();
	if (!Anim)
	{
		return;
	}

	if (!Animations.contains(Anim->GetKey()))
	{
		return;
	}

	std::shared_ptr<CAnimation2DSequence> Sequence(new CAnimation2DSequence);

	Sequence->SetAnimation2D(Anim);
	Sequence->SetPlayTime(PlayTime);
	Sequence->SetPlayRate(PlayRate);
	Sequence->SetLoop(bLoop);
	Sequence->SetReverse(bReverse);

	Animations.emplace(Anim->GetKey(), Sequence);

	if (!CurrentAnimation)
	{
		CurrentAnimation = Sequence;

		if (auto MeshComponent = UpdateComponent.lock();
			MeshComponent && CurrentAnimation)
		{
			if (auto Asset = CurrentAnimation->GetAsset().lock())
			{
				AnimationCBuffer->SetTextureType(Asset->GetTextureType());

				if (MeshComponent->SetTexture(0, 0, Asset->GetTexture()))
				{
					bUpdateEnable = true;
				}
			}
		}
	}
}

void CAnimation2DComponent::AddAnimation(const std::string& AnimKey, float PlayTime, float PlayRate, bool bLoop,
	bool bReverse)
{
	std::weak_ptr<CAnimation2D> WeakAnim;

	if (auto World = this->World.lock())
	{
		if (auto WorldAssetMgr = World->GetWorldAssetManager().lock())
		{
			WeakAnim = WorldAssetMgr->FindAnimation(AnimKey);
		}
	}

	auto Anim = WeakAnim.lock();
	if (!Anim)
	{
		return;
	}

	auto [It, Result] = Animations.try_emplace(Anim->GetKey(), std::shared_ptr<CAnimation2DSequence>(new CAnimation2DSequence));

	// Already exist.
	if (!Result)
	{
		return;
	}

	auto Sequence = It->second;

	Sequence->SetAnimation2D(Anim);
	Sequence->SetPlayTime(PlayTime);
	Sequence->SetPlayRate(PlayRate);
	Sequence->SetLoop(bLoop);
	Sequence->SetReverse(bReverse);

	if (!CurrentAnimation)
	{
		CurrentAnimation = Sequence;

		if (auto MeshComponent = UpdateComponent.lock();
			MeshComponent && CurrentAnimation)
		{
			if (auto Asset = CurrentAnimation->GetAsset().lock())
			{
				AnimationCBuffer->SetTextureType(Asset->GetTextureType());

				if (MeshComponent->SetTexture(0, 0, Asset->GetTexture()))
				{
					bUpdateEnable = true;
				}
			}
		}
	}
}

void CAnimation2DComponent::SetPlayTime(const std::string& AnimKey, float PlayTime)
{
	auto InnerKey = "Animation2D_" + AnimKey;

	auto It = Animations.find(InnerKey);
	if (It != Animations.end())
	{
		It->second->SetPlayTime(PlayTime);
	}
}

void CAnimation2DComponent::SetPlayRate(const std::string& AnimKey, float PlayRate)
{
	auto InnerKey = "Animation2D_" + AnimKey;

	auto It = Animations.find(InnerKey);
	if (It != Animations.end())
	{
		It->second->SetPlayRate(PlayRate);
	}
}

void CAnimation2DComponent::SetLoop(const std::string& AnimKey, bool bLoop)
{
	auto InnerKey = "Animation2D_" + AnimKey;

	auto It = Animations.find(InnerKey);
	if (It != Animations.end())
	{
		It->second->SetLoop(bLoop);
	}
}

void CAnimation2DComponent::SetReverse(const std::string& AnimKey, bool bReverse)
{
	auto InnerKey = "Animation2D_" + AnimKey;

	auto It = Animations.find(InnerKey);
	if (It != Animations.end())
	{
		It->second->SetReverse(bReverse);
	}
}

void CAnimation2DComponent::SetSymmetry(const std::string& AnimKey, bool bSymmetry)
{
	auto InnerKey = "Animation2D_" + AnimKey;

	auto It = Animations.find(InnerKey);
	if (It == Animations.end())
	{
		return;
	}

	It->second->SetSymmetry(bSymmetry);
}

void CAnimation2DComponent::ChangeAnimation(const std::string& AnimKey)
{
	auto InnerKey = "Animation2D_" + AnimKey;

	auto MeshComponent = UpdateComponent.lock();
	if (!MeshComponent)
	{
		return;
	}

	if (!CurrentAnimation
		|| CurrentAnimation->GetKey() == InnerKey)
	{
		return;
	}

	auto It = Animations.find(InnerKey);
	if (It == Animations.end())
	{
		return;
	}

	CurrentAnimation->Clear();

	CurrentAnimation = It->second;
	CurrentAnimation->Clear();

	if (auto Asset = CurrentAnimation->GetAsset().lock())
	{
		AnimationCBuffer->SetTextureType(Asset->GetTextureType());

		MeshComponent->SetTexture(0, 0, Asset->GetTexture());
	}
}

void CAnimation2DComponent::SetShader()
{
	AnimationCBuffer->SetTextureSymmetry(CurrentAnimation->GetSymmetry());

	if (auto Asset = CurrentAnimation->GetAsset().lock())
	{
		if (Asset->GetTextureType() == EAnimation2DTextureType::SpriteSheet)
		{
			if (auto Texture = Asset->GetTexture().lock())
			{
				const auto& TexFrame = Asset->GetFrame(CurrentAnimation->GetCurrFrame());
				const auto TexInfo = Texture->GetTexture();

				AnimationCBuffer->SetLTUV(
					TexFrame.Start.x / TexInfo->Width,
					TexFrame.Start.y / TexInfo->Height);

				AnimationCBuffer->SetRBUV(
					(TexFrame.Start.x + TexFrame.Size.x) / TexInfo->Width,
					(TexFrame.Start.y + TexFrame.Size.y) / TexInfo->Height);
			}
		}
	}

	AnimationCBuffer->UpdateBuffer();
}

EAnimation2DTextureType CAnimation2DComponent::GetTextureType() const
{
	if (auto Asset = CurrentAnimation->GetAsset().lock())
	{
		return Asset->GetTextureType();
	}

	return EAnimation2DTextureType::None;
}

int CAnimation2DComponent::GetCurrentFrame() const
{
	return CurrentAnimation->GetCurrFrame();
}

CAnimation2DComponent* CAnimation2DComponent::Clone() const
{
	return new CAnimation2DComponent(*this);
}

#pragma once
#include "CObjectComponent.h"

#include "CAnimation2DSequence.h"

class CCBufferAnimation2D;
class CMeshComponent;
class CAnimation2D;

class CAnimation2DComponent :
    public CObjectComponent
{
	friend CGameObject;

protected:
	CAnimation2DComponent() = default;
	CAnimation2DComponent(const CAnimation2DComponent& other);
	CAnimation2DComponent(CAnimation2DComponent&& other) noexcept;
	CAnimation2DComponent& operator=(const CAnimation2DComponent& other);
	CAnimation2DComponent& operator=(CAnimation2DComponent&& other) noexcept;

public:
	~CAnimation2DComponent() override = default;

	bool Init() override;
	void Update(const float DeltaTime) override;
	void PostUpdate(const float Delta) override;
	void Destroy() override;

	void SetUpdateComponent(const std::weak_ptr<CMeshComponent>& Component);

	void AddAnimation(const std::weak_ptr<CAnimation2D>& Animation, float PlayTime = 1.f, float PlayRate = 1.f, bool bLoop = false, bool bReverse = false);
	void AddAnimation(const std::string& AnimKey, float PlayTime = 1.f, float PlayRate = 1.f, bool bLoop = false, bool bReverse = false);

	void SetPlayTime(const std::string& AnimKey, float PlayTime);
	void SetPlayRate(const std::string& AnimKey, float PlayRate);
	void SetLoop(const std::string& AnimKey, bool bLoop);
	void SetReverse(const std::string& AnimKey, bool bReverse);
	void ChangeAnimation(const std::string& AnimKey);
	void SetShader();
	EAnimation2DTextureType GetTextureType() const;
	int GetCurrentFrame() const;

protected:
	CAnimation2DComponent* Clone() const override;

	std::weak_ptr<CMeshComponent> UpdateComponent;
	std::unordered_map <std::string, std::shared_ptr<CAnimation2DSequence>> Animations;

	std::shared_ptr<CAnimation2DSequence> CurrentAnimation;

	std::shared_ptr<CCBufferAnimation2D> AnimationCBuffer;

	bool bUpdateEnable = false;
};


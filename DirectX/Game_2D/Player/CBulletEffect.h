#pragma once
#include "Object/CGameObject.h"

class CAnimation2DComponent;
class CMeshComponent;

class CBulletEffect :
    public CGameObject
{
	friend class CWorld;
	friend CObject;

private:
	void FinishAnimation();

private:
	std::weak_ptr<CMeshComponent> MeshComponent;
	std::weak_ptr<CAnimation2DComponent> Animation2DComponent;

public:
	bool Init() override;
	void Update(const float DeltaTime) override;

protected:
	CBulletEffect* Clone() override;

protected:
	CBulletEffect() = default;
	CBulletEffect(const CBulletEffect& other);
	CBulletEffect(CBulletEffect&& other) noexcept;
	CBulletEffect& operator=(const CBulletEffect& other);
	CBulletEffect& operator=(CBulletEffect&& other) noexcept;

public:
	~CBulletEffect() override = default;
};


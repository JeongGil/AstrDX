#pragma once
#include <Object/CGameObject.h>

class CCameraComponent;
class CSceneComponent;
class CWorld;

class CCameraObject :
    public CGameObject
{
	friend CWorld;
	friend CObject;

protected:
	CCameraObject() = default;

public:
	~CCameraObject() override = default;

private:
	std::weak_ptr<CCameraComponent> CameraComponent;
	std::weak_ptr<CSceneComponent> Anchor;

public:
	bool Init() override;
	void PostUpdate(const float DeltaTime) override;

protected:
	CCameraObject* Clone() override;

public:
	[[nodiscard]] std::weak_ptr<CSceneComponent> GetAnchor() const
	{
		return Anchor;
	}

	void SetAnchor(const std::weak_ptr<CSceneComponent>& Anchor)
	{
		this->Anchor = Anchor;
	}

	[[nodiscard]] std::weak_ptr<CCameraComponent> GetCameraComponent() const
	{
		return CameraComponent;
	}
};


#pragma once
#include "Object/CGameObject.h"

class CMeshComponent;
class CCameraComponent;

class CMissile :
    public CGameObject
{
	friend class CWorld;
	friend CObject;

public:
	void SetCamera();

public:
	bool Init() override;
	void Update(float DeltaTime) override;

protected:
	CMissile* Clone() override;

private:
	std::weak_ptr<CMeshComponent> MeshComponent;
	std::weak_ptr<CCameraComponent> CameraComponent;

	float Distance = 1200;

protected:
	CMissile() = default;
	CMissile(const CMissile& other);
	CMissile(CMissile&& other) noexcept;

public:
	~CMissile() override = default;
};


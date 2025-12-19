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
	void Update(const float DeltaTime) override;

protected:
	CMissile* Clone() override;

private:
	std::weak_ptr<CMeshComponent> MeshComponent;
	std::weak_ptr<CCameraComponent> CameraComponent;

	float Distance = 1200;

protected:
	CMissile() = default;

	CMissile(const CMissile& other)
		: CGameObject(other),
		  MeshComponent(other.MeshComponent),
		  CameraComponent(other.CameraComponent),
		  Distance(other.Distance)
	{
	}

	CMissile(CMissile&& other) noexcept
		: CGameObject(std::move(other)),
		  MeshComponent(std::move(other.MeshComponent)),
		  CameraComponent(std::move(other.CameraComponent)),
		  Distance(other.Distance)
	{
	}

	CMissile& operator=(const CMissile& other)
	{
		if (this == &other)
			return *this;
		CGameObject::operator =(other);
		MeshComponent = other.MeshComponent;
		CameraComponent = other.CameraComponent;
		Distance = other.Distance;
		return *this;
	}

	CMissile& operator=(CMissile&& other) noexcept
	{
		if (this == &other)
			return *this;
		CGameObject::operator =(std::move(other));
		MeshComponent = std::move(other.MeshComponent);
		CameraComponent = std::move(other.CameraComponent);
		Distance = other.Distance;
		return *this;
	}

public:
	~CMissile() override = default;
};


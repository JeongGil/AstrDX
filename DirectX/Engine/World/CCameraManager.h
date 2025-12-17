#pragma once

#include "../EngineInfo.h"

class CCameraComponent;

class CCameraManager
{
	friend class CWorld;

public:
	void AddCamera(const std::string& Key, const std::weak_ptr<CCameraComponent>& Camera);
	void SetMainCamera(const std::string& Key);
	void SetMainCamera(const std::weak_ptr<CCameraComponent>& Camera);

	[[nodiscard]] std::weak_ptr<CCameraComponent> GetMainCamera() const
	{
		return MainCamera;
	}

	const FVector& GetMainCameraWorldPosition() const;
	const FVector& GetMainCameraAxis(EAxis::Type Axis) const;
	const FMatrix& GetViewMatrix()const;
	const FMatrix& GetProjectionMatrix()const;

private:
	std::unordered_multimap<std::string, std::weak_ptr<CCameraComponent>> Cameras;
	std::weak_ptr<CCameraComponent> MainCamera;

public:
	bool Init();
	void Update(const float DeltaTime);

private:
	CCameraManager() = default;

public:
	~CCameraManager() = default;
};


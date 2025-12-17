#include "CCameraManager.h"

#include "../Component/CCameraComponent.h"

void CCameraManager::AddCamera(const std::string& Key, const std::weak_ptr<CCameraComponent>& Camera)
{
	if (auto it = Cameras.find(Key); it == Cameras.end())
	{
		if (Cameras.empty())
		{
			MainCamera = Camera;
		}

		Cameras.emplace(Key, Camera);
	}
}

void CCameraManager::SetMainCamera(const std::string& Key)
{
	if (auto it = Cameras.find(Key); it != Cameras.end())
	{
		MainCamera = it->second;
	}
}

void CCameraManager::SetMainCamera(const std::weak_ptr<CCameraComponent>& Camera)
{
	MainCamera = Camera;
}

const FVector& CCameraManager::GetMainCameraWorldPosition() const
{
	if (auto Cam = MainCamera.lock())
	{
		return Cam->GetWorldPosition();
	}

	return FVector::Zero;
}

const FVector& CCameraManager::GetMainCameraAxis(EAxis::Type Axis) const
{
	if (auto Cam = MainCamera.lock())
	{
		return Cam->GetAxis(Axis);
	}

	return FVector::Zero;
}

const FMatrix& CCameraManager::GetViewMatrix() const
{
	if (auto Cam = MainCamera.lock())
	{
		return Cam->GetViewMatrix();
	}

	return FMatrix::Identity;
}

const FMatrix& CCameraManager::GetProjectionMatrix() const
{
	if (auto Cam = MainCamera.lock())
	{
		return Cam->GetProjectionMatrix();
	}

	return FMatrix::Identity;
}

bool CCameraManager::Init()
{
	return true;
}

void CCameraManager::Update(const float DeltaTime)
{
	auto Curr = Cameras.begin();
	const auto End = Cameras.end();
	while (Curr != End)
	{
		if (Curr->second.expired())
		{
			Curr = Cameras.erase(Curr);

			continue;
		}

		++Curr;
	}

	if (MainCamera.expired() && !Cameras.empty())
	{
		MainCamera = Cameras.begin()->second;
	}
}

#include "CCameraComponent.h"

#include "../CDevice.h"
#include "../World/CWorld.h"

void CCameraComponent::SetProjection(EProjectionType Type, float ViewAngleDegree, float Width, float Height,
	float ViewDistance)
{
	this->ProjectionType = Type;
	this->ViewAngleDegree = ViewAngleDegree;
	this->Width = Width;
	this->Height = Height;
	this->ViewDistance = ViewDistance;

	switch (Type)
	{
		case EProjectionType::Perspective:
		{
			ProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH(
				DirectX::XMConvertToRadians(ViewAngleDegree), Width / Height, 0.1f, ViewDistance);
			break;
		}
		case EProjectionType::Orthogonal:
		{
			ProjectionMatrix = DirectX::XMMatrixOrthographicOffCenterLH(
				Width / -2.f, Width / 2.f, Height / -2.f, Height / 2.f, 0.f, ViewDistance);
			break;
		}
	}
}

CCameraComponent* CCameraComponent::Clone() const
{
	return new CCameraComponent(*this);
}

bool CCameraComponent::Init()
{
	if (!CSceneComponent::Init())
	{
		return false;
	}

	FResolution Resolution = CDevice::GetInst()->GetResolution();
	SetProjection(EProjectionType::Perspective, 90, static_cast<float>(Resolution.Width), static_cast<float>(Resolution.Height), 1000);

	if (auto World = this->World.lock())
	{
		auto WeakCamMgr = World->GetCameraManager();
		if (auto CamMgr = WeakCamMgr.lock())
		{
			CamMgr->AddCamera(Key, std::dynamic_pointer_cast<CCameraComponent>(shared_from_this()));

			return true;
		}
	}

	return false;
}

void CCameraComponent::Update(const float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);
}

void CCameraComponent::PostUpdate(const float DeltaTime)
{
	/*
	How to apply a camera:
	Move all objects in the world by the same amount the camera moves to (0, 0, 0),
	and rotate the objects so that the direction the camera is looking at (the camera's Z-axis) becomes (0, 0, 1).
	When the camera is transformed into view space, the camera's X, Y, Z axes become:
	X-axis: 1, 0, 0
	Y-axis: 0, 1, 0
	Z-axis: 0, 0, 1
	Transforming the camera's X, Y, Z axes in world space into view space results in:
	WXx WXy WXz * View Matrix = 1, 0, 0
	WYx WYy WYz * View Matrix = 0, 1, 0
	WZx WZy WZz * View Matrix = 0, 0, 1
	If you multiply the 3x3 matrix composed of the camera's three world-space axes by the 3x3 rotation part of the view matrix, the result is an identity matrix.
	View Matrix = Inverse of:
		WXx WXy WXz
		WYx WYy WYz
		WZx WZy WZz
	The matrix above is orthogonal because each axis is at 90 degrees to the others. A matrix composed of axes that are mutually orthogonal is called an orthogonal matrix. For orthogonal matrices, the transpose matrix is equal to the inverse matrix.
	WXx WYx WZx 0
	WXy WYy WZy 0
	WXz WYz WZz 0
	0   0   0   1
	This is the rotational part of the view matrix.
	1  0  0  0
	0  1  0  0
	0  0  1  0
	-x -y -z 1
	This is the translational part of the view matrix.
	1  0  0  0   WXx WYx WZx 0   WXx  WYx  WZx  0
	0  1  0  0   WXy WYy WZy 0 = WXy  WYy  WZy  0
	0  0  1  0 * WXz WYz WZz 0   WXz  WYz  WZz  0
	-x -y -z 1   0   0   0   1   -X.P -Y.P -Z.P 1
	*/
	ViewMatrix.SetIdentity();

	for (int i = 0; i < EAxis::End; i++)
	{
		memcpy(&ViewMatrix[i], &WorldAxis[i], sizeof(FVector));
	}

	ViewMatrix.Transpose();

	for (int i = 0; i < EAxis::End; i++)
	{
		ViewMatrix[3][i] = -WorldPosition.Dot(WorldAxis[i]);
	}

	CSceneComponent::PostUpdate(DeltaTime);
}

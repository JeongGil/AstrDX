#pragma once

#include "CColliderBox2D.h"
#include "CColliderLine2D.h"
#include "CColliderSphere2D.h"

class CCollision
{
public:
	static bool CollideBox2DToBox2D(FVector& OutHitPoint, const CColliderBox2D* Src, const CColliderBox2D* Dest);
	static bool CollideAABB2DToAABB2D(FVector& OutHitPoint, const FBox2DInfo& Src, const FBox2DInfo& Dest);
	static bool CollideOBB2DToOBB2D(FVector& OutHitPoint, const FBox2DInfo& Src, const FBox2DInfo& Dest);

	static bool CollideSphere2DtoSphere2D(FVector& OutHitPoint, const CColliderSphere2D* Src, const CColliderSphere2D* Dest);
	static bool CollideSphere2DtoSphere2D(FVector& OutHitPoint, const FSphere2DInfo& Src, const FSphere2DInfo& Dest);

	static bool CollideBox2DToSphere2D(FVector& OutHitPoint, const CColliderBox2D* Box, const CColliderSphere2D* Sphere);
	static bool CollideBox2DToSphere2D(FVector& OutHitPoint, const FBox2DInfo& Box, const FSphere2DInfo& Sphere);

	static bool CollideBox2DToLine2D(FVector& OutHitPoint, CColliderBox2D* Box, CColliderLine2D* Line);
	static bool CollideBox2DToLine2D(FVector& OutHitPoint, const FBox2DInfo& Box, const FLine2DInfo& Line);

	static bool CollideLine2DToLine2D(FVector& OutHitPoint, CColliderLine2D* Src, CColliderLine2D* Dest);
	static bool CollideLine2DToLine2D(FVector& OutHitPoint, const FLine2DInfo& Src, const FLine2DInfo& Dest);

private:
	static bool AxisProjection(const FVector& CenterLine,
	                           const FVector& Axis, float SrcHalfExtent,
	                           const FVector* DestAxis, const FVector2& DestHalfExtent);
};


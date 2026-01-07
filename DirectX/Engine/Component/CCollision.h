#pragma once

#include "CColliderBox2D.h"

class CCollision
{
public:
	static bool CollideBox2DToBox2D(FVector& OutHitPoint, const CColliderBox2D* Src, const CColliderBox2D* Dest);
	static bool CollideAABB2DToAABB2D(FVector& OutHitPoint, const FBox2DInfo& Src, const FBox2DInfo& Dest);
	static bool CollideOBB2DToOBB2D(FVector& OutHitPoint, const FBox2DInfo& Src, const FBox2DInfo& Dest);

private:
	static bool AxisProjection(const FVector& CenterLine,
	                           const FVector& Axis, float SrcHalfExtent,
	                           const FVector* DestAxis, const FVector2& DestHalfExtent);
};


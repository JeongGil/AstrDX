#pragma once

#include "CColliderBox2D.h"
#include "CColliderLine2D.h"
#include "CColliderSphere2D.h"

namespace ECCWResult
{
	enum Type
	{
		CW = -1,
		Line,
		CCW,
	};
}

class CCollision
{
private:
	static float Clamp(float t);
	static FVector3 Perp2D(const FVector3& v);
	static FVector3 ClosestPointOnSegment2D(const FVector3& v1,
		const FVector3& v2, const FVector3& p, float* OutT = nullptr);
	static float ProjectBoxRadiusOnAxis(const FBox2DInfo& Box,
		const FVector3& AxisN);
	static FVector3 SupportPointBox2D(const FBox2DInfo& Box,
		const FVector3& Dir);
	static bool Manifold_BoxPoint2D(FCollisionManifold& Out,
		const FBox2DInfo& Box, const FVector3& Point);
	static bool ManifoldBox2DAxisOverlap(float& MinOverlap,
		FVector3& MinAxis, FVector3 Axis,
		const FBox2DInfo& Src, const FBox2DInfo& Dest);

public:
	static bool ManifoldBox2DToBox2D(FCollisionManifold& Out,
		const FBox2DInfo& Src, const FBox2DInfo& Dest);
	static bool ManifoldBox2DToBox2D(FCollisionManifold& Out,
		CColliderBox2D* Src, CColliderBox2D* Dest);
	static bool ManifoldSphere2DToSphere2D(FCollisionManifold& Out,
		const FSphere2DInfo& Src, const FSphere2DInfo& Dest);
	static bool ManifoldSphere2DToSphere2D(FCollisionManifold& Out,
		CColliderSphere2D* Src, CColliderSphere2D* Dest);
	static bool ManifoldBox2DToSphere2D(FCollisionManifold& Out,
		const FBox2DInfo& Box, const FSphere2DInfo& Sphere);
	static bool ManifoldBox2DToSphere2D(FCollisionManifold& Out,
		CColliderBox2D* Box, CColliderSphere2D* Sphere);
	static bool ManifoldSphere2DToBox2D(FCollisionManifold& Out,
		const FSphere2DInfo& Sphere, const FBox2DInfo& Box);
	static bool ManifoldSphere2DToBox2D(FCollisionManifold& Out,
		CColliderSphere2D* Sphere, CColliderBox2D* Box);

	static bool ManifoldSphere2DToLine2D(FCollisionManifold& Out,
		const FSphere2DInfo& Sphere, const FLine2DInfo& Line);
	static bool ManifoldSphere2DToLine2D(FCollisionManifold& Out,
		CColliderSphere2D* Sphere, CColliderLine2D* Line);
	static bool ManifoldLine2DToSphere2D(FCollisionManifold& Out,
		const FLine2DInfo& Line, const FSphere2DInfo& Sphere);
	static bool ManifoldLine2DToSphere2D(FCollisionManifold& Out,
		CColliderLine2D* Line, CColliderSphere2D* Sphere);

	static bool ManifoldBox2DToLine2D(FCollisionManifold& Out,
		const FBox2DInfo& Box, const FLine2DInfo& Line);
	static bool ManifoldBox2DToLine2D(FCollisionManifold& Out,
		CColliderBox2D* Box, CColliderLine2D* Line);
	static bool ManifoldLine2DToBox2D(FCollisionManifold& Out,
		const FLine2DInfo& Line, const FBox2DInfo& Box);
	static bool ManifoldLine2DToBox2D(FCollisionManifold& Out,
		CColliderLine2D* Line, CColliderBox2D* Box);

	static bool ManifoldLine2DToLine2D(FCollisionManifold& Out,
		const FLine2DInfo& Src, const FLine2DInfo& Dest);
	static bool ManifoldLine2DToLine2D(FCollisionManifold& Out,
		CColliderLine2D* Src, CColliderLine2D* Dest);

private:
	static bool GroundClamp(FVector3& Velocity, float InvMass,
		const FVector3& PushDir, const FVector3& Up);

public:
	static bool ResolveSlideStop2D(FVector3& SrcPos,
		FVector3& SrcVelocity, float SrcInvMass,
		FVector3& DestPos,
		FVector3& DestVelocity, float DestInvMass,
		const FCollisionManifold& Manifold);

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

	static bool CollideSphere2DToLine2D(FVector& OutHitPoint, CColliderSphere2D* Sphere, CColliderLine2D* Line);
	static bool CollideSphere2DToLine2D(FVector& OutHitPoint, const FSphere2DInfo& Sphere, const FLine2DInfo& Line);

	static bool CollideLine2DToLine2D(FVector& OutHitPoint, CColliderLine2D* Src, CColliderLine2D* Dest);
	static bool CollideLine2DToLine2D(FVector& OutHitPoint, const FLine2DInfo& Src, const FLine2DInfo& Dest);

	static bool CollideBox2DToPoint(FVector& OutHitPoint, const CColliderBox2D* Box, const FVector& Point);
	static bool CollideBox2DToPoint(FVector& OutHitPoint, const FBox2DInfo& Box, const FVector& Point);

	static bool CollideSphere2DToPoint(FVector& OutHitPoint, const CColliderSphere2D* Sphere, const FVector& Point);
	static bool CollideSphere2DToPoint(FVector& OutHitPoint, const FSphere2DInfo& Sphere, const FVector& Point);

	static bool CollideLine2DToPoint(FVector& OutHitPoint, const CColliderLine2D* Line, const FVector& Point);
	static bool CollideLine2DToPoint(FVector& OutHitPoint, const FLine2DInfo& Line, const FVector& Point);

private:
	static bool AxisProjection(const FVector& CenterLine,
	                           const FVector& Axis, float SrcHalfExtent,
	                           const FVector* DestAxis, const FVector2& DestHalfExtent);

	static ECCWResult::Type CCW2D(const FVector& A, const FVector& B, const FVector& C);

	static bool PointOnLine2D(const FVector& Point, const FLine2DInfo& Line);
	static bool PointOnLine2D(const FVector& Point, const FVector& LineStart, const FVector& LineEnd);
};


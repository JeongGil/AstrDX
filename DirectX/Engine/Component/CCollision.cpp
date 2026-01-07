#include "CCollision.h"

bool CCollision::CollideBox2DToBox2D(FVector& OutHitPoint, const CColliderBox2D* Src, const CColliderBox2D* Dest)
{
	if (!Src || !Dest)
	{
		return false;
	}

	if (Src->GetWorldRotation().IsZero() && Dest->GetWorldRotation().IsZero())
	{
		return CollideAABB2DToAABB2D(OutHitPoint, Src->GetInfo(), Dest->GetInfo());
	}
	else
	{
		return CollideOBB2DToOBB2D(OutHitPoint, Src->GetInfo(), Dest->GetInfo());
	}
}

bool CCollision::CollideAABB2DToAABB2D(FVector& OutHitPoint, const FBox2DInfo& Src, const FBox2DInfo& Dest)
{
	FVector SrcMin = Src.Center - Src.Axis[EAxis::X] * Src.HalfExtent.x - Src.Axis[EAxis::Y] * Src.HalfExtent.y;
	FVector SrcMax = Src.Center + Src.Axis[EAxis::X] * Src.HalfExtent.x + Src.Axis[EAxis::Y] * Src.HalfExtent.y;

	FVector DestMin = Dest.Center - Dest.Axis[EAxis::X] * Dest.HalfExtent.x - Dest.Axis[EAxis::Y] * Dest.HalfExtent.y;
	FVector DestMax = Dest.Center + Dest.Axis[EAxis::X] * Dest.HalfExtent.x + Dest.Axis[EAxis::Y] * Dest.HalfExtent.y;

	if (SrcMax.x < DestMin.x || SrcMin.x > DestMax.x)
	{
		return false;
	}

	if (SrcMax.y < DestMin.y || SrcMin.y > DestMax.y)
	{
		return false;
	}

	FVector IntersectMin(max(SrcMin.x, DestMin.x), max(SrcMin.y, DestMin.y), 0.f);
	FVector IntersectMax(min(SrcMax.x, DestMax.x), min(SrcMax.y, DestMax.y), 0.f);

	OutHitPoint = (IntersectMin + IntersectMax) * 0.5f;

	return true;
}

bool CCollision::CollideOBB2DToOBB2D(FVector& OutHitPoint, const FBox2DInfo& Src, const FBox2DInfo& Dest)
{
	FVector CenterLine = Src.Center - Dest.Center;

	// SAT

	// Src Axis
	if (!AxisProjection(CenterLine, Src.Axis[EAxis::X], Src.HalfExtent.x, Dest.Axis, Dest.HalfExtent))
	{
		return false;
	}
	if (!AxisProjection(CenterLine, Src.Axis[EAxis::Y], Src.HalfExtent.y, Dest.Axis, Dest.HalfExtent))
	{
		return false;
	}

	// Dest Axis
	if (!AxisProjection(CenterLine, Dest.Axis[EAxis::X], Dest.HalfExtent.x, Src.Axis, Src.HalfExtent))
	{
		return false;
	}
	if (!AxisProjection(CenterLine, Dest.Axis[EAxis::Y], Dest.HalfExtent.y, Src.Axis, Src.HalfExtent))
	{
		return false;
	}

	FVector SrcMin = Src.Center - Src.Axis[EAxis::X] * Src.HalfExtent.x - Src.Axis[EAxis::Y] * Src.HalfExtent.y;
	FVector SrcMax = Src.Center + Src.Axis[EAxis::X] * Src.HalfExtent.x + Src.Axis[EAxis::Y] * Src.HalfExtent.y;
	FVector DestMin = Dest.Center - Dest.Axis[EAxis::X] * Dest.HalfExtent.x - Dest.Axis[EAxis::Y] * Dest.HalfExtent.y;
	FVector DestMax = Dest.Center + Dest.Axis[EAxis::X] * Dest.HalfExtent.x + Dest.Axis[EAxis::Y] * Dest.HalfExtent.y;

	FVector IntersectMin(max(SrcMin.x, DestMin.x), max(SrcMin.y, DestMin.y), 0.f);
	FVector IntersectMax(min(SrcMax.x, DestMax.x), min(SrcMax.y, DestMax.y), 0.f);

	OutHitPoint = (IntersectMin + IntersectMax) * 0.5f;

	return true;
}

bool CCollision::CollideSphere2DtoSphere2D(FVector& OutHitPoint, const CColliderSphere2D* Src,
	const CColliderSphere2D* Dest)
{
	if (!Src || !Dest)
	{
		return false;
	}

	return CollideSphere2DtoSphere2D(OutHitPoint, Src->GetInfo(), Dest->GetInfo());
}

bool CCollision::CollideSphere2DtoSphere2D(FVector& OutHitPoint, const FSphere2DInfo& Src, const FSphere2DInfo& Dest)
{
	float Distance = Src.Center.Distance(Dest.Center);

	if (Distance > Src.Radius + Dest.Radius)
	{
		return false;
	}

	float HalfDiff = ((Src.Radius + Dest.Radius) - Distance) * 0.5f;

	FVector Dir = (Dest.Center - Src.Center).GetNormalized();

	OutHitPoint = Src.Center + Dir * (Src.Radius - HalfDiff);

	return true;
}

bool CCollision::CollideBox2DToSphere2D(FVector& OutHitPoint, const CColliderBox2D* Box, const CColliderSphere2D* Sphere)
{
	if (!Box || !Sphere)
	{
		return false;
	}

	return CollideBox2DToSphere2D(OutHitPoint, Box->GetInfo(), Sphere->GetInfo());
}

bool CCollision::CollideBox2DToSphere2D(FVector& OutHitPoint, const FBox2DInfo& Box, const FSphere2DInfo& Sphere)
{
	FVector CenterLine = Box.Center - Sphere.Center;
	FVector Axis = CenterLine.GetNormalized();

	if (!AxisProjection(CenterLine, Axis, Sphere.Radius, Box.Axis, Box.HalfExtent))
	{
		return false;
	}

	float CenterProjectionDistance = abs(CenterLine.Dot(Box.Axis[EAxis::X]));
	if (CenterProjectionDistance > Sphere.Radius + Box.HalfExtent.x)
	{
		return false;
	}

	CenterProjectionDistance = abs(CenterLine.Dot(Box.Axis[EAxis::Y]));
	if (CenterProjectionDistance > Sphere.Radius + Box.HalfExtent.y)
	{
		return false;
	}

	FVector SrcMin = Sphere.Center - FVector(Sphere.Radius, Sphere.Radius, 0.f);
	FVector SrcMax = Sphere.Center + FVector(Sphere.Radius, Sphere.Radius, 0.f);

	FVector DestMin = Box.Center - Box.Axis[EAxis::X] * Box.HalfExtent.x - Box.Axis[EAxis::Y] * Box.HalfExtent.y;
	FVector DestMax = Box.Center + Box.Axis[EAxis::X] * Box.HalfExtent.x + Box.Axis[EAxis::Y] * Box.HalfExtent.y;

	FVector IntersectMin = FVector(max(SrcMin.x, DestMin.x), max(SrcMin.y, DestMin.y), 0.f);
	FVector IntersectMax = FVector(min(SrcMax.x, DestMax.x), min(SrcMax.y, DestMax.y), 0.f);

	OutHitPoint = (IntersectMin + IntersectMax) * 0.5f;

	return true;
}

bool CCollision::CollideBox2DToLine2D(FVector& OutHitPoint, CColliderBox2D* Box, CColliderLine2D* Line)
{
	return false;
}

bool CCollision::CollideBox2DToLine2D(FVector& OutHitPoint, const FBox2DInfo& Box, const FLine2DInfo& Line)
{
	return false;
}

bool CCollision::CollideLine2DToLine2D(FVector& OutHitPoint, CColliderLine2D* Src, CColliderLine2D* Dest)
{
	return false;
}

bool CCollision::CollideLine2DToLine2D(FVector& OutHitPoint, const FLine2DInfo& Src, const FLine2DInfo& Dest)
{
	return false;
}

bool CCollision::AxisProjection(const FVector& CenterLine, const FVector& Axis, float SrcHalfExtent,
	const FVector* DestAxis, const FVector2& DestHalfExtent)
{
	float CenterProjDist = abs(CenterLine.Dot(Axis));

	float DestProjDist = abs(DestAxis[EAxis::X].Dot(Axis)) * DestHalfExtent.x +
		abs(DestAxis[EAxis::Y].Dot(Axis)) * DestHalfExtent.y;

	return SrcHalfExtent + DestProjDist > CenterProjDist;
}

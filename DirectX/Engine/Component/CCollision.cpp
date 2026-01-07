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

bool CCollision::AxisProjection(const FVector& CenterLine, const FVector& Axis, float SrcHalfExtent,
                                const FVector* DestAxis, const FVector2& DestHalfExtent)
{
	float CenterProjDist = abs(CenterLine.Dot(Axis));

	float DestProjDist = abs(DestAxis[EAxis::X].Dot(Axis)) * DestHalfExtent.x +
		abs(DestAxis[EAxis::Y].Dot(Axis)) * DestHalfExtent.y;

	return SrcHalfExtent + DestProjDist > CenterProjDist;
}

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

bool CCollision::CollideSphere2DToLine2D(FVector& OutHitPoint, CColliderSphere2D* Sphere, CColliderLine2D* Line)
{
	if (!Sphere || !Line)
	{
		return false;
	}

	return CollideSphere2DToLine2D(OutHitPoint, Sphere->GetInfo(), Line->GetInfo());
}

bool CCollision::CollideSphere2DToLine2D(FVector& OutHitPoint, const FSphere2DInfo& Sphere, const FLine2DInfo& Line)
{
	auto LineVec = Line.End - Line.Start;
	float LineLength = LineVec.Length();
	LineVec.Normalize();

	auto M = Line.Start - Sphere.Center;

	float b = 2.f * LineVec.Dot(M);
	float c = M.Dot(M) - Sphere.Radius * Sphere.Radius;

	float Det = b * b - 4.f * c;

	if (Det < 0.f)
	{
		return false;
	}

	Det = sqrtf(Det);

	float t1 = (-b - Det) * 0.5f;
	float t2 = (-b + Det) * 0.5f;

	if (t1 < 0.f && t2 < 0.f)
	{
		return false;
	}

	bool Result = false;

	if (t1 >= 0.f && t1 <= LineLength
		|| t2 >= 0.f && t2 <= LineLength)
	{
		Result = true;
	}
	else
	{
		float Length1 = (Sphere.Center - Line.Start).Length();
		float Length2 = (Sphere.Center - Line.End).Length();

		if (Length1 <= Sphere.Radius && Length2 <= Sphere.Radius)
		{
			Result = true;
		}
	}

	if (Result)
	{
		float HitDist = min(t1, t2);

		if (HitDist < 0.f)
		{
			HitDist = max(t1, t2);
		}

		OutHitPoint = Line.Start + LineVec * HitDist;
	}

	return Result;
}

bool CCollision::CollideBox2DToPoint(FVector& OutHitPoint, const CColliderBox2D* Box, const FVector& Point)
{
	if (!Box)
	{
		return false;
	}

	return CollideBox2DToPoint(OutHitPoint, Box->GetInfo(), Point);
}

bool CCollision::CollideBox2DToPoint(FVector& OutHitPoint, const FBox2DInfo& Box, const FVector& Point)
{
	FVector CenterLine = Point - Box.Center;

	float Dist = abs(CenterLine.Dot(Box.Axis[EAxis::X]));
	if (Dist > Box.HalfExtent.x)
	{
		return false;
	}

	Dist = abs(CenterLine.Dot(Box.Axis[EAxis::Y]));
	if (Dist > Box.HalfExtent.y)
	{
		return false;
	}

	OutHitPoint = Point;

	return true;
}

ECCWResult::Type CCollision::CCW2D(const FVector& A, const FVector& B, const FVector& C)
{
	FVector v = B - A;
	FVector w = C - A;

	float Cross = v.x * w.y - v.y * w.x;

	if (Cross > 0.f)
	{
		return ECCWResult::CCW;
	}
	else if (Cross < 0.f)
	{
		return ECCWResult::CW;
	}
	else
	{
		return ECCWResult::Line;
	}
}

bool CCollision::PointOnLine2D(const FVector& Point, const FLine2DInfo& Line)
{
	return PointOnLine2D(Point, Line.Start, Line.End);
}

bool CCollision::PointOnLine2D(const FVector& Point, const FVector& LineStart, const FVector& LineEnd)
{
	float MinX, MinY, MaxX, MaxY;

	if (LineStart.x < LineEnd.x)
	{
		MinX = LineStart.x;
		MaxX = LineEnd.x;
	}
	else
	{
		MinX = LineEnd.x;
		MaxX = LineStart.x;
	}

	if (LineStart.y < LineEnd.y)
	{
		MinY = LineStart.y;
		MaxY = LineEnd.y;
	}
	else
	{
		MinY = LineEnd.y;
		MaxY = LineStart.y;
	}

	if (Point.x < MinX || Point.x > MaxX)
	{
		return false;
	}

	if (Point.y < MinY || Point.y > MaxY)
	{
		return false;
	}

	return true;
}

bool CCollision::CollideLine2DToLine2D(FVector& OutHitPoint, CColliderLine2D* Src, CColliderLine2D* Dest)
{
	if (!Src || !Dest)
	{
		return false;
	}

	return CollideLine2DToLine2D(OutHitPoint, Src->GetInfo(), Dest->GetInfo());
}

bool CCollision::CollideLine2DToLine2D(FVector& OutHitPoint, const FLine2DInfo& Src, const FLine2DInfo& Dest)
{
	int ccw1 = CCW2D(Src.Start, Src.End, Dest.Start);
	int ccw2 = CCW2D(Src.Start, Src.End, Dest.End);
	int ccw3 = CCW2D(Dest.Start, Dest.End, Src.Start);
	int ccw4 = CCW2D(Dest.Start, Dest.End, Src.End);

	if (ccw1 * ccw2 < 0 && ccw3 * ccw4 < 0)
	{
		FVector v = Src.Start - Src.End;
		FVector w = Dest.Start - Dest.End;

		float Det = v.x * w.y - v.y * w.x;

		OutHitPoint.x =
			((Src.Start.x * Src.End.y - Src.Start.y * Src.End.x) * (Dest.Start.x - Dest.End.x)
			- (Dest.Start.x - Dest.End.y - Dest.Start.y * Dest.End.x) * (Src.Start.x - Src.End.x)) / Det;

		OutHitPoint.y =
			((Src.Start.x * Src.End.y - Src.Start.y * Src.End.x) * (Dest.Start.y - Dest.End.y)
			- (Dest.Start.x - Dest.End.y - Dest.Start.y * Dest.End.x) * (Src.Start.y - Src.End.y)) / Det;

		return true;
	}

	if (ccw1 == 0 && PointOnLine2D(Dest.Start, Src))
	{
		OutHitPoint = Dest.Start;
		return true;
	}

	if (ccw2 == 0 && PointOnLine2D(Dest.End, Src))
	{
		OutHitPoint = Dest.End;
		return true;
	}

	if (ccw3 == 0 && PointOnLine2D(Src.Start, Dest))
	{
		OutHitPoint = Src.Start;
		return true;
	}

	if (ccw4 == 0 && PointOnLine2D(Src.End, Dest))
	{
		OutHitPoint = Src.End;
		return true;
	}

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

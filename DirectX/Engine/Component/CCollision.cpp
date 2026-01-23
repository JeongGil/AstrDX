#include "CCollision.h"

float CCollision::Clamp(float t)
{
	if (t < 0.f)
		return 0.f;

	else if (t > 1.f)
		return 1.f;

	return t;
}

FVector3 CCollision::Perp2D(const FVector3& v)
{
	return FVector3(-v.y, v.x, 0.f);
}

FVector3 CCollision::ClosestPointOnSegment2D(const FVector3& v1,
	const FVector3& v2, const FVector3& p, float* OutT)
{
	FVector3    v = v2 - v1;
	float   Dot = v.Dot(v);

	float   t = 0.f;

	if (Dot > 1e-8f)
	{
		t = (p - v1).Dot(v) / Dot;
		t = Clamp(t);
	}

	if (OutT)
		*OutT = t;

	return v1 + v * t;
}

float CCollision::ProjectBoxRadiusOnAxis(const FBox2DInfo& Box,
	const FVector3& AxisN)
{
	return fabsf(AxisN.Dot(Box.Axis[EAxis::X])) * Box.HalfExtent.x +
		fabsf(AxisN.Dot(Box.Axis[EAxis::Y])) * Box.HalfExtent.y;
}

FVector3 CCollision::SupportPointBox2D(const FBox2DInfo& Box,
	const FVector3& Dir)
{
	float   DotX = Box.Axis[EAxis::X].Dot(Dir);
	float   DotY = Box.Axis[EAxis::Y].Dot(Dir);

	float   x = -1.f;
	float   y = -1.f;

	if (DotX >= 0.f)
		x = 1.f;

	if (DotY >= 0.f)
		y = 1.f;

	return Box.Center + Box.Axis[EAxis::X] * (Box.HalfExtent.x * x) +
		Box.Axis[EAxis::Y] * (Box.HalfExtent.y * y);
}

bool CCollision::Manifold_BoxPoint2D(FCollisionManifold& Out,
	const FBox2DInfo& Box, const FVector3& Point)
{
	FVector3    v = Point - Box.Center;

	float   lx = v.Dot(Box.Axis[EAxis::X]);
	float   ly = v.Dot(Box.Axis[EAxis::Y]);

	float   dx = Box.HalfExtent.x - fabsf(lx);
	float   dy = Box.HalfExtent.y - fabsf(ly);

	if (dx < 0.f || dy < 0.f)
		return false;

	// 가장 얇은 축으로 밀어내는 방향 선택
	if (dx < dy)
	{
		float   Sign = (lx > 0.f) ? 1.f : -1.f;
		Out.Normal = Box.Axis[EAxis::X] * Sign;
		Out.Penetration = dx;
	}

	else
	{
		float   Sign = (ly > 0.f) ? 1.f : -1.f;
		Out.Normal = Box.Axis[EAxis::Y] * Sign;
		Out.Penetration = dy;
	}

	Out.Normal.Normalize();
	Out.ContactPoint = Point;

	return true;
}

bool CCollision::ManifoldBox2DAxisOverlap(float& MinOverlap,
	FVector3& MinAxis, FVector3 Axis,
	const FBox2DInfo& Src, const FBox2DInfo& Dest)
{
	Axis.Normalize();

	float   SrcDist = ProjectBoxRadiusOnAxis(Src, Axis);
	float   DestDist = ProjectBoxRadiusOnAxis(Dest, Axis);

	FVector3    CenterLine = Dest.Center - Src.Center;

	float   Dist = fabsf(CenterLine.Dot(Axis));

	float   Overlap = (SrcDist + DestDist) - Dist;

	if (Overlap < 0.f)
		return false;

	if (Overlap < MinOverlap)
	{
		MinOverlap = Overlap;
		MinAxis = Axis;
	}

	return true;
}

bool CCollision::ManifoldBox2DToBox2D(FCollisionManifold& Out,
	const FBox2DInfo& Src, const FBox2DInfo& Dest)
{
	FVector3    CenterLine = Dest.Center - Src.Center;

	float   MinOverlap = FLT_MAX;
	FVector3    MinAxis;

	if (!ManifoldBox2DAxisOverlap(MinOverlap, MinAxis,
		Src.Axis[EAxis::X], Src, Dest))
		return false;

	if (!ManifoldBox2DAxisOverlap(MinOverlap, MinAxis,
		Src.Axis[EAxis::Y], Src, Dest))
		return false;

	if (!ManifoldBox2DAxisOverlap(MinOverlap, MinAxis,
		Dest.Axis[EAxis::X], Src, Dest))
		return false;

	if (!ManifoldBox2DAxisOverlap(MinOverlap, MinAxis,
		Dest.Axis[EAxis::Y], Src, Dest))
		return false;

	// Normal은 Src -> Dest 방향이어야 한다.
	if (CenterLine.Dot(MinAxis) < 0.f)
		MinAxis = MinAxis * -1.f;

	Out.Normal = MinAxis;
	Out.Penetration = MinOverlap;

	// 접촉점을 구한다.
	FVector3    SrcPoint = SupportPointBox2D(Src, MinAxis);
	FVector3    DestPoint = SupportPointBox2D(Dest, MinAxis * -1.f);
	Out.ContactPoint = (SrcPoint + DestPoint) * 0.5f;

	return true;
}

bool CCollision::ManifoldBox2DToBox2D(FCollisionManifold& Out,
	CColliderBox2D* Src, CColliderBox2D* Dest)
{
	if (!Src || !Dest)
		return false;

	return ManifoldBox2DToBox2D(Out, Src->GetInfo(), Dest->GetInfo());
}

bool CCollision::ManifoldSphere2DToSphere2D(FCollisionManifold& Out, const FSphere2DInfo& Src, const FSphere2DInfo& Dest)
{
	FVector3    CenterLine = Dest.Center - Src.Center;

	float   Dist = CenterLine.Length();
	float   RadiusSum = Src.Radius + Dest.Radius;

	if (Dist > RadiusSum)
		return false;

	// 1e-6f는 1 * 10^-6 = 0.000001
	// 이 경우 두 원이 겹치있다고 판단하는 것이다.
	if (Dist < 1e-6f)
	{
		Out.Normal = FVector3(1.f, 0.f, 0.f);
		Out.Penetration = RadiusSum;
		Out.ContactPoint = Src.Center;
		return true;
	}

	// Src -> Dest 방향으로 정규화 한다.
	Out.Normal = CenterLine / Dist;
	Out.Penetration = RadiusSum - Dist;

	Out.ContactPoint = Src.Center + Out.Normal *
		(Src.Radius - Out.Penetration * 0.5f);

	return true;
}

bool CCollision::ManifoldSphere2DToSphere2D(FCollisionManifold& Out, CColliderSphere2D* Src, CColliderSphere2D* Dest)
{
	if (!Src || !Dest)
		return false;

	return ManifoldSphere2DToSphere2D(Out, Src->GetInfo(),
		Dest->GetInfo());
}

bool CCollision::ManifoldBox2DToSphere2D(FCollisionManifold& Out, const FBox2DInfo& Box, const FSphere2DInfo& Sphere)
{
	// Box -> Sphere 향하는 벡터
	FVector3    CenterLine = Sphere.Center - Box.Center;

	float   lx = CenterLine.Dot(Box.Axis[EAxis::X]);
	float   ly = CenterLine.Dot(Box.Axis[EAxis::Y]);

	float   clx = lx, cly = ly;

	if (clx < -Box.HalfExtent.x)
		clx = -Box.HalfExtent.x;

	else if (clx > Box.HalfExtent.x)
		clx = Box.HalfExtent.x;

	if (cly < -Box.HalfExtent.y)
		cly = -Box.HalfExtent.y;

	else if (cly > Box.HalfExtent.y)
		cly = Box.HalfExtent.y;

	FVector3    Closest = Box.Center + Box.Axis[EAxis::X] * clx +
		Box.Axis[EAxis::Y] * cly;
	FVector3    Diff = Sphere.Center - Closest;

	float   Dist = Diff.Length();

	if (Dist > Sphere.Radius)
	{
		if (Dist > Sphere.Radius + 1e-6f)
			return false;
	}

	if (Dist < 1e-6f)
	{
		float   dx = Box.HalfExtent.x - fabsf(lx);
		float   dy = Box.HalfExtent.y - fabsf(ly);

		if (dx < dy)
		{
			float   Sign = (lx >= 0.f) ? 1.f : -1.f;
			Out.Normal = Box.Axis[EAxis::X] * Sign;
			Out.Penetration = Sphere.Radius + dx;
		}

		else
		{
			float   Sign = (ly >= 0.f) ? 1.f : -1.f;
			Out.Normal = Box.Axis[EAxis::Y] * Sign;
			Out.Penetration = Sphere.Radius + dy;
		}

		Out.Normal.Normalize();
		Out.ContactPoint = Sphere.Center;

		return true;
	}

	Out.Normal = (Sphere.Center - Closest) / Dist;
	Out.Penetration = Sphere.Radius - Dist;
	Out.ContactPoint = Closest;

	return Out.Penetration >= 0.f;
}

bool CCollision::ManifoldBox2DToSphere2D(FCollisionManifold& Out, CColliderBox2D* Box, CColliderSphere2D* Sphere)
{
	if (!Box || !Sphere)
		return false;

	return ManifoldBox2DToSphere2D(Out, Box->GetInfo(),
		Sphere->GetInfo());
}

bool CCollision::ManifoldSphere2DToBox2D(FCollisionManifold& Out, const FSphere2DInfo& Sphere, const FBox2DInfo& Box)
{
	FCollisionManifold  Result;
	if (!ManifoldBox2DToSphere2D(Result, Box, Sphere))
		return false;

	Out.ContactPoint = Result.ContactPoint;
	Out.Penetration = Result.Penetration;
	Out.Normal = Result.Normal * -1.f;

	return true;
}

bool CCollision::ManifoldSphere2DToBox2D(FCollisionManifold& Out, CColliderSphere2D* Sphere, CColliderBox2D* Box)
{
	if (!Box || !Sphere)
		return false;

	return ManifoldSphere2DToBox2D(Out, Sphere->GetInfo(),
		Box->GetInfo());
}

bool CCollision::ManifoldSphere2DToLine2D(FCollisionManifold& Out, const FSphere2DInfo& Sphere, const FLine2DInfo& Line)
{
	FVector3    Closest = ClosestPointOnSegment2D(Line.Start,
		Line.End, Sphere.Center);

	// Sphere -> Line
	FVector3    CenterLine = Closest - Sphere.Center;
	float   Dist = CenterLine.Length();

	if (Dist > Sphere.Radius)
		return false;

	if (Dist < 1e-6f)
	{
		FVector3    Dir = Line.End - Line.Start;
		Dir.Normalize();

		FVector3    Normal = Perp2D(Dir);
		Normal.Normalize();

		Out.Normal = Normal;
		Out.Penetration = Sphere.Radius;
		Out.ContactPoint = Closest;
		return true;
	}

	Out.Normal = CenterLine / Dist;
	Out.Penetration = Sphere.Radius - Dist;
	Out.ContactPoint = Closest;

	return true;
}

bool CCollision::ManifoldSphere2DToLine2D(FCollisionManifold& Out, CColliderSphere2D* Sphere, CColliderLine2D* Line)
{
	if (!Sphere || !Line)
		return false;

	return ManifoldSphere2DToLine2D(Out, Sphere->GetInfo(),
		Line->GetInfo());
}

bool CCollision::ManifoldLine2DToSphere2D(FCollisionManifold& Out, const FLine2DInfo& Line, const FSphere2DInfo& Sphere)
{
	FCollisionManifold  Result;
	if (!ManifoldSphere2DToLine2D(Result, Sphere, Line))
		return false;

	Out.ContactPoint = Result.ContactPoint;
	Out.Penetration = Result.Penetration;
	Out.Normal = Result.Normal * -1.f;

	return true;
}

bool CCollision::ManifoldLine2DToSphere2D(FCollisionManifold& Out, CColliderLine2D* Line, CColliderSphere2D* Sphere)
{
	if (!Sphere || !Line)
		return false;

	return ManifoldLine2DToSphere2D(Out, Line->GetInfo(),
		Sphere->GetInfo());
}

bool CCollision::ManifoldBox2DToLine2D(FCollisionManifold& Out, const FBox2DInfo& Box, const FLine2DInfo& Line)
{
	FVector3    Hit;

	if (!CollideBox2DToLine2D(Hit, Box, Line))
		return false;

	// 라인 끝점이 박스 내부로 들어온 경우
	FCollisionManifold  Result;

	if (CollideBox2DToPoint(Hit, Box, Line.Start))
	{
		if (!Manifold_BoxPoint2D(Result, Box, Line.Start))
			return false;

		Out.ContactPoint = Line.Start;
		Out.Penetration = Result.Penetration;
		Out.Normal = Result.Normal;

		return true;
	}

	else if (CollideBox2DToPoint(Hit, Box, Line.End))
	{
		if (!Manifold_BoxPoint2D(Result, Box, Line.End))
			return false;

		Out.ContactPoint = Line.End;
		Out.Penetration = Result.Penetration;
		Out.Normal = Result.Normal;

		return true;
	}

	// 라인이 박스를 관통하는 경우
	FVector3    Dir = Line.End - Line.Start;
	float   LineLength = Dir.Length();

	if (LineLength < 1e-6f)
		return false;

	Dir.Normalize();
	FVector3    Normal = Perp2D(Dir);
	Normal.Normalize();

	// 박스 중심의 직선에 대한 부호거리
	float   SignDist = (Box.Center - Line.Start).Dot(Normal);

	float   NormalDist = ProjectBoxRadiusOnAxis(Box, Normal);
	float   Pen = NormalDist - fabsf(SignDist);

	if (Pen < 0.f)
		Pen = 0.f;

	Out.Normal = (SignDist > 0.f) ? Normal * -1.f : Normal;
	Out.Normal.Normalize();

	Out.Penetration = Pen;
	Out.ContactPoint = Hit;

	return true;
}

bool CCollision::ManifoldBox2DToLine2D(FCollisionManifold& Out, CColliderBox2D* Box, CColliderLine2D* Line)
{
	if (!Box || !Line)
		return false;

	return ManifoldBox2DToLine2D(Out, Box->GetInfo(),
		Line->GetInfo());
}

bool CCollision::ManifoldLine2DToBox2D(FCollisionManifold& Out, const FLine2DInfo& Line, const FBox2DInfo& Box)
{
	FCollisionManifold  Result;
	if (!ManifoldBox2DToLine2D(Result, Box, Line))
		return false;

	Out.ContactPoint = Result.ContactPoint;
	Out.Penetration = Result.Penetration;
	Out.Normal = Result.Normal * -1.f;

	return true;
}

bool CCollision::ManifoldLine2DToBox2D(FCollisionManifold& Out, CColliderLine2D* Line, CColliderBox2D* Box)
{
	if (!Box || !Line)
		return false;

	return ManifoldLine2DToBox2D(Out, Line->GetInfo(),
		Box->GetInfo());
}

bool CCollision::ManifoldLine2DToLine2D(FCollisionManifold& Out, const FLine2DInfo& Src, const FLine2DInfo& Dest)
{
	FVector3    Hit;

	if (!CollideLine2DToLine2D(Hit, Src, Dest))
		return false;

	Out.ContactPoint = Hit;
	Out.Penetration = 0.f;

	FVector3    SrcDir = Src.End - Src.Start;
	FVector3    DestDir = Dest.End - Dest.Start;

	if (DestDir.Length() < 1e-6f)
		Out.Normal = FVector3(1.f, 0.f, 0.f);

	else
	{
		DestDir.Normalize();
		FVector3    Normal = Perp2D(DestDir);
		Normal.Normalize();

		FVector3    SrcMid = (Src.Start + Src.End) * 0.5f;
		FVector3    DestMid = (Dest.Start + Dest.End) * 0.5f;

		// Src -> Dest 방향 맞추기
		if ((DestMid - SrcMid).Dot(Normal) > 0.f)
			Normal *= -1.f;

		Out.Normal = Normal;
	}

	return true;
}

bool CCollision::ManifoldLine2DToLine2D(FCollisionManifold& Out, CColliderLine2D* Src, CColliderLine2D* Dest)
{
	if (!Src || !Dest)
		return false;

	return ManifoldLine2DToLine2D(Out, Src->GetInfo(),
		Dest->GetInfo());
}

bool CCollision::GroundClamp(FVector3& Velocity, float InvMass,
	const FVector3& PushDir, const FVector3& Up)
{
	if (InvMass <= 0.f)
		return false;

	FVector3    PushDirNormal = PushDir;
	PushDirNormal.Normalize();

	if (PushDirNormal.Dot(Up) <= 0.8f)
		return false;

	float   vInto = Velocity.Dot(PushDirNormal);

	if (vInto < 0.f)
		Velocity -= PushDirNormal * vInto;

	return true;
}

// Mass 값은 고정된 물체는 0 아니면 1을 넣어주면 된다.
// 질량이 모두 같다고 정의하고 하는 방식이다. 그게 아니라면
// SrcInvMass나 DestInvMass에 1.f / Mass 를 넣어주면 된다.
bool CCollision::ResolveSlideStop2D(FVector3& SrcPos,
	FVector3& SrcVelocity, float SrcInvMass,
	FVector3& DestPos, FVector3& DestVelocity, float DestInvMass,
	const FCollisionManifold& Manifold)
{
	if (Manifold.Penetration <= 0.f)
		return false;

	FVector3    Normal = Manifold.Normal;
	Normal.Normalize();

	// 방향 고정
	if ((DestPos - SrcPos).Dot(Normal) < 0.f)
		Normal *= -1.f;

	const float Slop = 0.02f;  // 떨림 방지
	const float Percent = 0.4f; // 보정 강도
	const float MaxCor = 0.5f;
	// 속도임계. 0 근처 떨림 방지.
	const float VelEps = 0.001f;

	float   InvMass = SrcInvMass + DestInvMass;

	if (InvMass <= 0.f)
		return false;

	// 위치 보정(겹침 제거)
	float   Pen = 0.f;

	if (Manifold.Penetration - Slop > 0.f)
		Pen = Manifold.Penetration - Slop;

	float   CorMag = (Pen * Percent) / InvMass;
	if (CorMag > MaxCor)
		CorMag = MaxCor;

	FVector3    Correction = Normal * CorMag;

	SrcPos -= Correction * SrcInvMass;
	DestPos += Correction * DestInvMass;

	// 속도 보정(미끄러짐 가능)
	FVector3    Velocity = SrcVelocity - DestVelocity;
	float   vn = Velocity.Dot(Normal);

	if (vn > VelEps)
	{
		FVector3    dv = Normal * (vn / InvMass);

		SrcVelocity -= dv * SrcInvMass;
		DestVelocity += dv * DestInvMass;
	}

	// 바닥 떨림 방지
	FVector3    PushDir = Normal * -1.f;
	PushDir.Normalize();

	FVector3    Up = FVector3(0.f, 1.f, 0.f);

	// 바닥 판정을 위해 밀어내는 방향을 위쪽 방향벡터와 계산하여
	// 일장 크기 이상일 경우 바닥으로 판정되게 한다.
	/*bool    Grounded = (PushDir.Dot(Up) > 0.8f);

	if (Grounded)
	{
		float   vInto = SrcVelocity.Dot(PushDir);
		if (vInto < 0.f)
			SrcVelocity -= PushDir * vInto;
	}*/
	bool    Grounded = false;

	Grounded |= GroundClamp(SrcVelocity, SrcInvMass, -Normal, Up);
	Grounded |= GroundClamp(DestVelocity, DestInvMass, Normal, Up);

	return Grounded;
}

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

bool CCollision::CollideSphere2DToPoint(FVector& OutHitPoint, const CColliderSphere2D* Sphere, const FVector& Point)
{
	if (!Sphere)
	{
		return false;
	}

	return CollideSphere2DToPoint(OutHitPoint, Sphere->GetInfo(), Point);
}

bool CCollision::CollideSphere2DToPoint(FVector& OutHitPoint, const FSphere2DInfo& Sphere, const FVector& Point)
{
	OutHitPoint = Point;

	return Sphere.Center.SqrDistance(Point) <= Sphere.Radius * Sphere.Radius;
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

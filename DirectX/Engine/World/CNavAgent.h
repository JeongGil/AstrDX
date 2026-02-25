#pragma once

#include "../EngineInfo.h"

class CNavAgent
{

public:
	CNavAgent() = default;
	virtual ~CNavAgent() = 0;

protected:
	std::vector<FVector3> Paths;
	float AcceptDistance = 10.f;
	int PathIndex = -1;
	float MoveDist = 0.f;

public:
	void SetAcceptDistance(float Dist)
	{
		AcceptDistance = Dist;
	}

	void StartPathPoint()
	{
		Paths.clear();
		PathIndex = 0;
	}

	void AddPathPoint(const FVector& Pos)
	{
		Paths.push_back(Pos);
	}

	virtual void StartPath() = 0;
};


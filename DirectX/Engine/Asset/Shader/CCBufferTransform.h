#pragma once

#include "CConstantBufferData.h"

class CCBufferTransform :
	public CConstantBufferData
{
public:
	bool Init() override;
	void UpdateBuffer() override;
	CCBufferTransform* Clone() override;

public:
	void SetWorldMatrix(const FMatrix& World) { Data.World = World; }
	void SetViewMatrix(const FMatrix& View) { Data.View = View; }
	void SetProjectionMatrix(const FMatrix& Proj) { Data.Proj = Proj; }
	void SetPivotSize(const FVector& PivotSize) { Data.PivotSize = PivotSize; }

private:
	FCBufferTransformData Data;

public:
	CCBufferTransform() = default;
	CCBufferTransform(const CCBufferTransform& other) = default;
	CCBufferTransform(CCBufferTransform&& other) noexcept = default;
	CCBufferTransform& operator=(const CCBufferTransform& other) = default;
	CCBufferTransform& operator=(CCBufferTransform&& other) noexcept = default;
	~CCBufferTransform() override = default;
};


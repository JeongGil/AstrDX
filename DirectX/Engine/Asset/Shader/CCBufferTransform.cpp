#include "CCBufferTransform.h"

#include "CConstantBuffer.h"

bool CCBufferTransform::Init()
{
	SetConstantBuffer("Transform");

	return true;
}

void CCBufferTransform::UpdateBuffer()
{
	Data.WV = Data.World * Data.View;
	Data.WVP = Data.WV * Data.Proj;

	// DX: row-major
	// HLSL: col-major
	// Transpose matrix to use matrix at HLSL.
	Data.World.Transpose();
	Data.View.Transpose();
	Data.Proj.Transpose();
	Data.WV.Transpose();
	Data.WVP.Transpose();

	if (auto CBuffer = Buffer.lock())
	{
		CBuffer->Update(&Data);
	}
}

CCBufferTransform* CCBufferTransform::Clone()
{
	return new CCBufferTransform(*this);
}

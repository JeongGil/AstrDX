#include "Share.hlsli"

struct VS_INPUT_TEX
{
	float3 Pos : POSITION0;
	float2 UV : TEXCOORD;
};

cbuffer CBUIDefault : register(b10)
{
    // Color to multiply with the image color.
	float4 cbBrushTint;
	float4 cbWidgetColor;
	float2 cbBrushLTUV;
	float2 cbBrushRBUV;
	int cbBrushAnimEnable;
	int cbBrushTextureEnable;
	float2 cbBrushEmpty;
};

float2 UpdateAnimationUI(float2 UV)
{
	if (cbBrushAnimEnable == 0)
		return UV;
    
	float2 Result = (float2) 0.f;
    
	if (UV.x == 0.f)
		Result.x = cbBrushLTUV.x;
	else
		Result.x = cbBrushRBUV.x;
    
	if (UV.y == 0.f)
		Result.y = cbBrushLTUV.y;
	else
		Result.y = cbBrushRBUV.y;

	return Result;
}

VS_OUTPUT_TEX UIDefaultVS(VS_INPUT_TEX input)
{
	VS_OUTPUT_TEX output = (VS_OUTPUT_TEX) 0;
    
	float3 Pos = input.Pos - cbPivotSize;
    
	output.Pos = mul(float4(Pos, 1.f), cbWVP);
	output.UV = UpdateAnimationUI(input.UV);

	return output;
}

struct PS_OUTPUT_COLOR
{
	float4 Color : SV_TARGET;
};

Texture2D tbBaseTexture : register(t0);

PS_OUTPUT_COLOR UIDefaultPS(VS_OUTPUT_TEX input)
{
	PS_OUTPUT_COLOR output = (PS_OUTPUT_COLOR) 0;
    
	float4 Color = float4(1.f, 1.f, 1.f, 1.f);
    
	if (cbBrushTextureEnable == 1)
		Color = tbBaseTexture.Sample(sbPoint, input.UV);
    
	output.Color = Color * cbBrushTint * cbWidgetColor;
    
	return output;
}

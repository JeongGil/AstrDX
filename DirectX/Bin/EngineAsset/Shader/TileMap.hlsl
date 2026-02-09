#include "Share.hlsli"

struct VS_INPUT_TEX
{
	float3 Pos : POSITION0;
	float2 UV : TEXCOORD;
};

struct VS_INPUT_INSTANCING_TEX
{
	float3 Pos : POSITION0;
	float2 UV : TEXCOORD;
    
	float4 WVP0 : INSTANCE_WVP0;
	float4 WVP1 : INSTANCE_WVP1;
	float4 WVP2 : INSTANCE_WVP2;
	float4 WVP3 : INSTANCE_WVP3;
	float2 LTUV : INSTANCE_UV0;
	float2 RBUV : INSTANCE_UV1;
};

cbuffer CBTileMap : register(b10)
{
	float2 cbTileLTUV;
	float2 cbTileRBUV;
};

Texture2D tbBackTexture : register(t0);
Texture2D tbTileTexture : register(t1);

float2 ComputeTileUV(float2 UV)
{
	float2 Result;
    
	if (UV.x > 0.f)
	{
		Result.x = cbTileRBUV.x;
	}
	else
	{
		Result.x = cbTileLTUV.x;
	}
    
	if (UV.y > 0.f)
	{
		Result.y = cbTileLTUV.y;
	}
	else
	{
		Result.y = cbTileRBUV.y;
	}
    
	return Result;
}

VS_OUTPUT_TEX TileMapVS(VS_INPUT_TEX input)
{
	VS_OUTPUT_TEX output = (VS_OUTPUT_TEX) 0;
    
	float3 Pos = input.Pos - cbPivotSize;
    
	output.Pos = mul(float4(Pos, 1.f), cbWVP);
	output.UV = ComputeTileUV(input.UV);
	
	return output;
}

struct PS_OUTPUT_COLOR
{
	float4 Color : SV_TARGET;
};

PS_OUTPUT_COLOR TileMapPS(VS_OUTPUT_TEX input)
{
	PS_OUTPUT_COLOR output = (PS_OUTPUT_COLOR) 0;
    
	float4 TextureColor = tbTileTexture.Sample(sbLinear, input.UV);
    
	output.Color = TextureColor;
    
	return output;
}

// Instancing
float2 ComputeInstancingTileUV(float2 UV, float2 LT, float2 RB)
{
	float2 Result;
    
	if (UV.x > 0.f)
	{
		Result.x = RB.x;
	}
	else
	{
		Result.x = LT.x;
	}
    
	if (UV.y > 0.f)
	{
		Result.y = LT.y;
	}
	else
	{
		Result.y = RB.y;
	}
    
	return Result;
}

VS_OUTPUT_TEX TileMapInstancingVS(VS_INPUT_INSTANCING_TEX input)
{
	VS_OUTPUT_TEX output = (VS_OUTPUT_TEX) 0;
    
	matrix WVP = matrix(input.WVP0, input.WVP1, input.WVP2, input.WVP3);
    
	output.Pos = mul(float4(input.Pos, 1.f), WVP);
	output.UV = ComputeInstancingTileUV(input.UV, input.LTUV, input.RBUV);
	
	return output;
}

PS_OUTPUT_COLOR TileMapInstancingPS(VS_OUTPUT_TEX input)
{
	PS_OUTPUT_COLOR output = (PS_OUTPUT_COLOR) 0;
    
	float4 TextureColor = tbTileTexture.Sample(sbLinear, input.UV);
    
	output.Color = TextureColor;
    
	return output;
}

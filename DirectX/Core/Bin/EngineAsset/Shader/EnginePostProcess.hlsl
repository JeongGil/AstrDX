#include "Share.hlsli"

#define MAX_BLUR_COUNT  16

cbuffer CBBlur : register(b10)
{
	float2 cbTexelSize;
	int cbBlurCount;
	float cbBlueEmpty;
	float4 cbBlurWeight[(MAX_BLUR_COUNT + 1 + 3) / 4];
};


struct PS_OUTPUT_COLOR
{
	float4 Color : SV_TARGET;
};

Texture2D tbBlendTarget : register(t0);

PS_OUTPUT_COLOR BlurPS(VS_OUTPUT_TEX input)
{
	PS_OUTPUT_COLOR output = (PS_OUTPUT_COLOR) 0;
    
	float4 SrcColor = tbBlendTarget.Sample(sbPoint, input.UV);
        
	if (SrcColor.a == 0.f)
	{
		clip(-1);
	}
    
	SrcColor *= cbBlurWeight[0][0];
    
	uint i = 0;
	float4 TargetColor;
    
    [loop]
	for (i = 1; i <= cbBlurCount; ++i)
	{
		float2 UV = input.UV + float2(cbTexelSize.x * i, 0.f);
        
		TargetColor = tbBlendTarget.Sample(sbPoint, UV);
        
		uint Index1 = i / 4u;
		uint Index2 = i % 4u;
        
        //if (TargetColor.a > 0.f)
		SrcColor += TargetColor * cbBlurWeight[Index1][Index2];
        
		UV = input.UV - float2(cbTexelSize.x * i, 0.f);
        
		TargetColor = tbBlendTarget.Sample(sbPoint, UV);
        
        //if (TargetColor.a > 0.f)
		SrcColor += TargetColor * cbBlurWeight[Index1][Index2];
	}
    
    [loop]
	for (i = 1; i <= cbBlurCount; ++i)
	{
		float2 UV = input.UV + float2(0.f, cbTexelSize.y * i);
        
		TargetColor = tbBlendTarget.Sample(sbPoint, UV);
        
		uint Index1 = i / 4u;
		uint Index2 = i % 4u;
        
        //if (TargetColor.a > 0.f)
		SrcColor += TargetColor * cbBlurWeight[Index1][Index2];
        
		UV = input.UV - float2(0.f, cbTexelSize.y * i);
        
		TargetColor = tbBlendTarget.Sample(sbPoint, UV);
        
        //if (TargetColor.a > 0.f)
		SrcColor += TargetColor * cbBlurWeight[Index1][Index2];
	}
    
	SrcColor /= 2.f;
    
	output.Color = SrcColor;
    
	return output;
}

// Write content commonly used in Shader

/*
 * Register of shader:
 * Input register: Used to pass vertex information. Used as a combination of NameNumber.
 * Output register: Used to pass completed vertex information. DirectX uses this information to determine the position on the screen and prepare for color output.
 * Constant register: Used to pass general data from C++ code to the shader.
 * Temporary register: A register used during computations.
 */

// A feature that transfers frequently changing values from the CPU to the GPU.
cbuffer CBTransform : register(b0)
{
	matrix cbWorld;
	matrix sbView;
	matrix cbProj;
	matrix cbWV;
	matrix cbWVP;
}

cbuffer CBMaterial : register(b1)
{
	float4 cbBaseColor;
	float cbOpacity;
	float3 cbMaterialEmpty;
}

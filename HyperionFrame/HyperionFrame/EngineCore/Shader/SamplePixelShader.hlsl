// 通过像素着色器传递的每个像素的颜色数据。
struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float4 posW : POSITION;
	float3 color : COLOR0;
	float3 norm : NORMAL0;
	float2 uv : TEXCOORD0;
};

cbuffer EyePos : register(b2)
{
	float3 EyePos;
}

// (内插)颜色数据的传递函数。
float4 main(PixelShaderInput input) : SV_TARGET
{
	float3 inPos = input.posW.xyz;
	float3 eyeDir = normalize(EyePos - inPos);
	float angle = dot(input.norm, eyeDir);
	return float4(input.color * angle, 1.0f);
}

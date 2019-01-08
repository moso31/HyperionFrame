// ͨ��������ɫ�����ݵ�ÿ�����ص���ɫ���ݡ�
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

// (�ڲ�)��ɫ���ݵĴ��ݺ�����
float4 main(PixelShaderInput input) : SV_TARGET
{
	float3 inPos = input.posW.xyz;
	float3 eyeDir = normalize(EyePos - inPos);
	float angle = dot(input.norm, eyeDir);
	return float4(input.color * angle, 1.0f);
}

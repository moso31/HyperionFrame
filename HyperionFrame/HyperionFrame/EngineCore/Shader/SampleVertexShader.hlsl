// �洢���ڹ��ɼ���ͼ�ε��������������Ⱦ���ĳ�����������
cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
	matrix model;
	matrix view;
	matrix projection;
};

cbuffer VertBufferData : register(b1)
{
	float3 vertColor;
}

// ����������ɫ�������ÿ����������ݡ�
struct VertexShaderInput
{
	float3 pos : POSITION;
	float3 norm : NORMAL;
	float2 uv : TEXCOORD;
};

// ͨ��������ɫ�����ݵ�ÿ�����ص���ɫ���ݡ�
struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float4 posW : POSITION;
	float3 color : COLOR0;
	float3 norm : NORMAL0;
	float2 uv : TEXCOORD0;
};

// ������ GPU ��ִ�ж��㴦��ļ���ɫ����
PixelShaderInput main(VertexShaderInput input)
{
	PixelShaderInput output;
	float4 pos = float4(input.pos, 1.0f);

	// ������λ��ת��ΪͶӰ�ռ䡣
	pos = mul(pos, model);
	output.posW = pos;
	pos = mul(pos, view);
	pos = mul(pos, projection);
	output.pos = pos;

	// �����޸ĵش�����ɫ��
	output.color = vertColor;
	float4 norm = float4(input.norm, 0.0f);
	output.norm = normalize(mul(norm, model).xyz);
	output.uv = input.uv;

	return output;
}

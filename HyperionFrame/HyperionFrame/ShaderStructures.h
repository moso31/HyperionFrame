#pragma once
#include <DirectXMath.h>

using namespace DirectX;

// 用于向顶点着色器发送 MVP 矩阵的常量缓冲区。
struct ModelViewProjectionConstantBuffer
{
	XMFLOAT4X4 model;
	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;
};

// 用于向顶点着色器发送每个顶点的数据。
struct VertexPositionColor
{
	XMFLOAT3 pos;
	XMFLOAT3 color;
};

struct VertexPCT
{
	XMFLOAT3 pos;
	XMFLOAT3 color;
	XMFLOAT2 uv;
};
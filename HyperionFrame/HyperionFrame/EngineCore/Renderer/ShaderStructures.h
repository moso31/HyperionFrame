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

// 向顶点着色器传递Mesh的数据。
struct CBufferMeshData
{
	XMFLOAT3 vertColor;
};

// 向着色器传递摄像机坐标，以计算法线相关内容。
struct CBufferEyePos
{
	XMFLOAT3 eyePos;
};

// 用于向顶点着色器发送每个顶点的数据。
struct VertexPC
{
	XMFLOAT3 pos;
	XMFLOAT3 color;
};

struct VertexPNT
{
	XMFLOAT3 pos;
	XMFLOAT3 norm;
	XMFLOAT2 uv;
};
#pragma once
#include "HyperionMath.h"

// 用于向顶点着色器发送 MVP 矩阵的常量缓冲区。
struct ModelViewProjectionConstantBuffer
{
	HFloat4x4 model;
	HFloat4x4 view;
	HFloat4x4 projection;
};

// 向顶点着色器传递Mesh的数据。
struct CBufferMeshData
{
	HFloat3 vertColor;
};

struct CBufferLineData
{
	HFloat3 lineColor;
};

// 向着色器传递摄像机坐标，以计算法线相关内容。
struct CBufferEyePos
{
	HFloat3 eyePos;
};

// 用于向顶点着色器发送每个顶点的数据。
struct VertexPC
{
	HFloat3 pos;
	HFloat3 color;
};

struct VertexPNT
{
	HFloat3 pos;
	HFloat3 norm;
	HFloat2 uv;
};
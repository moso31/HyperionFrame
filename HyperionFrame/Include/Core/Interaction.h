#pragma once
#include "HMaterial.h"

class SurfaceInteraction
{
public:
	SurfaceInteraction();
	~SurfaceInteraction();

	void ComputeScatterFunctions(Ray ray);

public:
	// normal vector.
	XMFLOAT3 n;
	// 出射（交点-摄像机）向量
	XMFLOAT3 wo;
	// 交互的物体对象
	Shape* shape;
};

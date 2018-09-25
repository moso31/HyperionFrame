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
	// ���䣨����-�����������
	XMFLOAT3 wo;
	// �������������
	Shape* shape;
};

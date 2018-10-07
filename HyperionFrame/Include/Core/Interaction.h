#pragma once
#include "HMaterial.h"

class SurfaceInteraction
{
public:
	SurfaceInteraction();
	SurfaceInteraction(const XMFLOAT3 &p, const XMFLOAT2 &uv, const XMFLOAT3 &wo, const XMFLOAT3& dpdu, const XMFLOAT3& dpdv, Shape* shape);
	~SurfaceInteraction();

	void ComputeScatterFunctions();

public:
	XMFLOAT3 p;	
	XMFLOAT3 n;
	XMFLOAT3 wo;

	XMFLOAT3 dpdu, dpdv;
	XMFLOAT2 uv;

	Shape* shape;
};

#pragma once
#include "HMaterial.h"

class Interaction
{
public:
	Interaction() {}
	Interaction(const XMFLOAT3 &p) : p(p) {}
	Interaction(const XMFLOAT3 &p, const XMFLOAT3 &wo) : p(p), n(n) {}
	Interaction(const XMFLOAT3 &p, const XMFLOAT3 &n, const XMFLOAT3 &wo) : p(p), n(n), wo(wo) {}

public:
	XMFLOAT3 p;
	XMFLOAT3 n;
	XMFLOAT3 wo;
};

class SurfaceInteraction : public Interaction
{
public:
	SurfaceInteraction();
	SurfaceInteraction(const XMFLOAT3 &p, const XMFLOAT2 &uv, const XMFLOAT3 &wo, const XMFLOAT3& dpdu, const XMFLOAT3& dpdv, Shape* shape);
	~SurfaceInteraction();

	void ComputeScatterFunctions();

public:
	XMFLOAT3 dpdu, dpdv;
	XMFLOAT2 uv;

	Shape* shape;
};

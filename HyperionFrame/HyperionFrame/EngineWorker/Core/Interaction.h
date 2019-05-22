#pragma once
#include "HEngineMath.h"

class Interaction
{
public:
	Interaction() {}
	Interaction(const HFloat3 &p) : p(p) {}
	Interaction(const HFloat3 &p, const HFloat3 &wo) : p(p), n(n) {}
	Interaction(const HFloat3 &p, const HFloat3 &n, const HFloat3 &wo) : p(p), n(n), wo(wo) {}
	virtual ~Interaction() {};

	Ray SpawnRay(const HFloat3 &d) const;
	Ray SpawnRayTo(const HFloat3 &p1) const;
	Ray SpawnRayTo(const Interaction &it) const;

public:
	HFloat3 p;
	HFloat3 n;
	HFloat3 wo;
};

class SurfaceInteraction : public Interaction
{
public:
	SurfaceInteraction();
	SurfaceInteraction(const HFloat3 &p, const HFloat2 &uv, const HFloat3 &wo, const HFloat3& dpdu, const HFloat3& dpdv, HShape* shape);
	~SurfaceInteraction();

	void ComputeScatterFunctions();

public:
	HFloat3 dpdu, dpdv;
	HFloat2 uv;

	HShape* shape;
	BSDF* bsdf;
};

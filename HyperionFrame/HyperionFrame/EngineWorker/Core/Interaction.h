#pragma once
#include "HEngineMath.h"

class Interaction
{
public:
	Interaction(const HFloat3 &p = 0.0f, const HFloat3 &n = 0.0f, const HFloat3 &wo = 0.0f) : p(p), n(n), wo(wo) {}
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
	SurfaceInteraction() = default;
	SurfaceInteraction(const HFloat3 &p, const HFloat2 &uv, const HFloat3 &wo, const HFloat3& dpdu, const HFloat3& dpdv, HShape* shape);
	~SurfaceInteraction() {}

	void ComputeScatterFunctions();

public:
	HFloat3 dpdu, dpdv;
	HFloat2 uv;

	HShape* shape;
	BSDF* bsdf;
};

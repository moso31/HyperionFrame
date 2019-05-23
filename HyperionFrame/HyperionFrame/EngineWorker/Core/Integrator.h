#pragma once
#include "HEngineMath.h"
#include "HSampler.h"

class Integrator
{
public:
	Integrator() {};
	virtual ~Integrator() {};
	virtual HFloat3 Li(const Ray& ray, HSampler& sampler, const HScene& scene, HInt depth, vector<Segment>* out_debug_rayTraceData) = 0;

private:
	const HInt maxDepth = 5;
};
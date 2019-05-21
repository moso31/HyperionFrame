#pragma once
#include "HEngineMath.h"
#include "HSampler.h"

class Integrator
{
public:
	Integrator() {};
	virtual ~Integrator() {};
	virtual XMCOLOR3 Li(const Ray& ray, HSampler& sampler, const HScene& scene, int depth, vector<Segment>* out_debug_rayTraceData) = 0;

private:
	const int maxDepth = 5;
};
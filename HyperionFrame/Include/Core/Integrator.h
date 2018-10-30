#pragma once
#include "HScene.h"
#include "HSampler.h"

class Integrator
{
public:
	Integrator() {};
	virtual ~Integrator() {};
	virtual XMCOLOR3 Li(const Ray& ray, HSampler& sampler, const HScene& scene, int depth) = 0;

private:
	const int maxDepth = 5;
};
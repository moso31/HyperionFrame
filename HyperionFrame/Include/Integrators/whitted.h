#pragma once
#include "Integrator.h"

class WhittedIntegrator : public Integrator
{
public:
	XMCOLOR3 Li(const Ray& ray, const HScene& scene, int depth);

private:
	const int maxDepth = 5;
};
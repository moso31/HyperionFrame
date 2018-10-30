#pragma once
#include "Integrator.h"

class WhittedIntegrator : public Integrator
{
public:
	XMCOLOR3 Li(const Ray& ray, HSampler& sampler, const HScene& scene, int depth);
	XMCOLOR3 SpecularReflect(const Ray& ray, HSampler& sampler, const SurfaceInteraction& isect, const HScene& scene, int depth);
	XMCOLOR3 SpecularTransmit(const Ray& ray, HSampler& sampler, const SurfaceInteraction& isect, const HScene& scene, int depth);

private:
	const int maxDepth = 5;
};
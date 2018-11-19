#pragma once
#include "Integrator.h"

class WhittedIntegrator : public Integrator
{
public:
	XMCOLOR3 Li(const Ray& ray, HSampler& sampler, const HScene& scene, int depth);
	XMCOLOR3 SpecularReflect(const Ray& ray,const SurfaceInteraction& isect, const HScene& scene, HSampler& sampler, int depth);
	XMCOLOR3 SpecularTransmit(const Ray& ray, const SurfaceInteraction& isect, const HScene& scene, HSampler& sampler, int depth);

private:
	const static int maxDepth = 5;
};
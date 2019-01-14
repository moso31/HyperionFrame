#pragma once
#include "Integrator.h"

class WhittedIntegrator : public Integrator
{
public:
	XMCOLOR3 Li(const Ray& ray, HSampler& sampler, const HScene& scene, int depth, vector<Ray>* out_debug_rayTraceList = nullptr);
	XMCOLOR3 SpecularReflect(const Ray& ray,const SurfaceInteraction& isect, const HScene& scene, HSampler& sampler, int depth, vector<Ray>* out_debug_rayTraceList = nullptr);
	XMCOLOR3 SpecularTransmit(const Ray& ray, const SurfaceInteraction& isect, const HScene& scene, HSampler& sampler, int depth, vector<Ray>* out_debug_rayTraceList = nullptr);

private:
	const static int maxDepth = 5;
};
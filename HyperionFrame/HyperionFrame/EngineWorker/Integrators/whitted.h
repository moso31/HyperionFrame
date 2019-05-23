#pragma once
#include "Integrator.h"

class WhittedIntegrator : public Integrator
{
public:
	HFloat3 Li(const Ray& ray, HSampler& sampler, const HScene& scene, HInt depth, vector<Segment>* out_debug_rayTraceData = nullptr);
	HFloat3 SpecularReflect(const Ray& ray,const SurfaceInteraction& isect, const HScene& scene, HSampler& sampler, HInt depth, vector<Segment>* out_debug_rayTraceData = nullptr);
	HFloat3 SpecularTransmit(const Ray& ray, const SurfaceInteraction& isect, const HScene& scene, HSampler& sampler, HInt depth, vector<Segment>* out_debug_rayTraceData = nullptr);

private:
	const static HInt maxDepth = 5;
};
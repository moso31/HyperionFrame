#pragma once
#include "Integrator.h"

class WhittedIntegrator : public Integrator
{
public:
	XMCOLOR3 Li(const Ray& ray, HSampler& sampler, const HScene& scene, int depth, vector<Segment>* out_debug_rayTraceData = nullptr);
	XMCOLOR3 SpecularReflect(const Ray& ray,const SurfaceInteraction& isect, const HScene& scene, HSampler& sampler, int depth, vector<Segment>* out_debug_rayTraceData = nullptr);
	XMCOLOR3 SpecularTransmit(const Ray& ray, const SurfaceInteraction& isect, const HScene& scene, HSampler& sampler, int depth, vector<Segment>* out_debug_rayTraceData = nullptr);

private:
	const static int maxDepth = 5;
};
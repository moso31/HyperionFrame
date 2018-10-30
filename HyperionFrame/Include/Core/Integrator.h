#pragma once
#include "HScene.h"

class Integrator
{
public:
	Integrator() {};
	virtual ~Integrator() {};
	virtual XMCOLOR3 Li(const Ray& ray, const HScene& scene, int depth) = 0;
	XMCOLOR3 SpecularReflect(const Ray& ray, const SurfaceInteraction& isect, const HScene& scene, int depth);
	XMCOLOR3 SpecularTransmit(const Ray& ray, const SurfaceInteraction& isect, const HScene& scene, int depth);

private:
	const int maxDepth = 5;
};
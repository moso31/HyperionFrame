#pragma once
#include "HScene.h"

class WhittedIntegrator
{
public:
	XMCOLOR3 Li(const Ray& ray, const HScene& scene, int depth);

private:
};
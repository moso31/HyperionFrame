#include "HPointLight.h"

HPointLight::HPointLight()
{
}

HPointLight::~HPointLight()
{
}

XMFLOAT3 HPointLight::Sample_Li(const SurfaceInteraction & isect, XMFLOAT3 & out_wi, VisibilityTester * out_vis)
{
	return XMFLOAT3();
}

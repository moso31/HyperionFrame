#include "HPointLight.h"

HPointLight::HPointLight()
{
}

HPointLight::~HPointLight()
{
}

HFloat3 HPointLight::Sample_Li(const Interaction & isect, HFloat3 & out_wi, VisibilityTester * out_vis)
{
	HFloat3 wiV = (translation - isect.p).Normalize();
	out_wi = (translation - isect.p).Normalize();

	*out_vis = VisibilityTester(isect, Interaction(translation));

	return m_intensityColor / (translation - isect.p).LengthSq();
}

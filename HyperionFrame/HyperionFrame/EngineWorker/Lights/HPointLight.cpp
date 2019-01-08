#include "HPointLight.h"

HPointLight::HPointLight()
{
}

HPointLight::~HPointLight()
{
}

XMCOLOR3 HPointLight::Sample_Li(const Interaction & isect, XMFLOAT3 & out_wi, VisibilityTester * out_vis)
{
	XMVECTOR hitPosV = XMLoadFloat3(&isect.p);
	XMVECTOR lightPosV = XMLoadFloat3(&translation);
	XMVECTOR wiV = XMVector3Normalize(lightPosV - hitPosV);
	XMStoreFloat3(&out_wi, wiV);

	XMVECTOR intensityV = XMLoadFloat3(&m_intensityColor);
	XMCOLOR3 result;
	XMStoreFloat3(&result, intensityV / XMVector3LengthSq(lightPosV - hitPosV));

	*out_vis = VisibilityTester(isect, Interaction(translation));

	return result;
}

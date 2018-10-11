#pragma once
#include "HLight.h"

class HPointLight : public HLight
{
public:
	HPointLight();
	~HPointLight();

	XMFLOAT3 Sample_Li(const Interaction& isect, XMFLOAT3& out_wi, VisibilityTester* out_vis);

private:
	XMFLOAT3 m_intensityColor;
};

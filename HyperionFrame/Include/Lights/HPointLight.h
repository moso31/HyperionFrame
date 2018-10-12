#pragma once
#include "HLight.h"

class HPointLight : public HLight
{
public:
	HPointLight();
	~HPointLight();

	XMCOLOR3 Sample_Li(const Interaction& isect, XMFLOAT3& out_wi, VisibilityTester* out_vis);

private:
	XMCOLOR3 m_intensityColor;
};

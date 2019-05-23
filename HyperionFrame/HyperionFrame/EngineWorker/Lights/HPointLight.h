#pragma once
#include "HLight.h"

class HPointLight : public HLight
{
public:
	HPointLight();
	~HPointLight();

	HFloat3 GetIntensity() { return m_intensityColor; }
	void SetIntensity(HFloat3 value) { m_intensityColor = value; }
	void SetIntensity(HFloat R, HFloat G, HFloat B) { m_intensityColor = { R, G, B }; }

	HFloat3 Sample_Li(const Interaction& isect, HFloat3& out_wi, VisibilityTester* out_vis);

private:
	HFloat3 m_intensityColor;
};

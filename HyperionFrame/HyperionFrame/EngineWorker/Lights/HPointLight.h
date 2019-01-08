#pragma once
#include "HLight.h"

class HPointLight : public HLight
{
public:
	HPointLight();
	~HPointLight();

	XMCOLOR3 GetIntensity() { return m_intensityColor; }
	void SetIntensity(XMCOLOR3 value) { m_intensityColor = value; }
	void SetIntensity(float R, float G, float B) { m_intensityColor = { R, G, B }; }

	XMCOLOR3 Sample_Li(const Interaction& isect, XMFLOAT3& out_wi, VisibilityTester* out_vis);

private:
	XMCOLOR3 m_intensityColor;
};

#pragma once
#include "HMaterial.h"

class HMatteMaterial : public HMaterial
{
public:
	HMatteMaterial();
	HMatteMaterial(const XMCOLOR3& _Kd, const float& _sigma);
	~HMatteMaterial();

	void ComputeScatterFunction(SurfaceInteraction* si);

	XMCOLOR3 GetDiffuse() { return Kd; }
	void SetDiffuse(XMCOLOR3 diffuse) { Kd = diffuse; }

private:
	XMCOLOR3 Kd;
	float sigma;
};

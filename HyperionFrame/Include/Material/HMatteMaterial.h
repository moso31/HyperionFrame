#pragma once
#include "HMaterial.h"

class HMatteMaterial : public HMaterial
{
public:
	HMatteMaterial();
	HMatteMaterial(const XMCOLOR3& _Kd, const float& _sigma);
	~HMatteMaterial();

	void ComputeScatterFunction(SurfaceInteraction* si);

private:
	XMCOLOR3 Kd;
	float sigma;
};

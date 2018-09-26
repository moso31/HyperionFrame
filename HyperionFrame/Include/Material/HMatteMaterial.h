#pragma once
#include "HMaterial.h"

using namespace HIFrameMath;

class HMatteMaterial : public HMaterial
{
public:
	HMatteMaterial();
	HMatteMaterial(const XMFLOAT3& _Kd, const float& _sigma);
	~HMatteMaterial();

	void ComputeScatterFunction(SurfaceInteraction* si);

private:
	XMFLOAT3 Kd;
	float sigma;
};

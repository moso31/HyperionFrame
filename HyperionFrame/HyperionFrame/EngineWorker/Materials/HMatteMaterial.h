#pragma once
#include "HMaterial.h"

class HMatteMaterial : public HMaterial
{
public:
	HMatteMaterial();
	HMatteMaterial(const HFloat3& _Kd, const HFloat& _sigma);
	~HMatteMaterial();

	void ComputeScatterFunction(SurfaceInteraction* si);

	HFloat3 GetDiffuse() { return Kd; }
	void SetDiffuse(HFloat3 diffuse) { Kd = diffuse; }

private:
	HFloat3 Kd;
	HFloat sigma;
};

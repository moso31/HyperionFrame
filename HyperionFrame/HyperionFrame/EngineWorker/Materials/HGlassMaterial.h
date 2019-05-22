#pragma once
#include "HMaterial.h"

class HGlassMaterial : public HMaterial
{
public:
	HGlassMaterial();
	HGlassMaterial(const HFloat3 & _Kr, const HFloat3 & _Kt, HFloat _eta);
	~HGlassMaterial();

	void ComputeScatterFunction(SurfaceInteraction* si);

private:
	HFloat3 Kr, Kt;
	float eta;
};

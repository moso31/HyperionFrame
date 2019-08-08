#pragma once
#include "HMaterial.h"

class HMirrorMaterial : public HMaterial
{
public:
	HMirrorMaterial();
	HMirrorMaterial(const HFloat3& _Kr);
	~HMirrorMaterial();

	void ComputeScatterFunction(SurfaceInteraction* si);

private:
	HFloat3 Kr;
};

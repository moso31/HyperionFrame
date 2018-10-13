#pragma once
#include "HMath.h"

class HMaterial
{
public:
	HMaterial();
	virtual ~HMaterial();

	virtual void ComputeScatterFunction(SurfaceInteraction* si) = 0;

private:

};

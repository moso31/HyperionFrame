#pragma once
#include "HMath.h"

using namespace HIFrameMath;

class HMaterial
{
public:
	HMaterial();
	~HMaterial();

	virtual void ComputeScatterFunction(SurfaceInteraction* si) = 0;

private:

};

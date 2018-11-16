#pragma once
#include "Interaction.h"
#include "Reflection.h"

class HMaterial
{
public:
	HMaterial();
	virtual ~HMaterial();

	virtual void ComputeScatterFunction(SurfaceInteraction* si) = 0;

private:

};

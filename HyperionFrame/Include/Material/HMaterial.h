#pragma once
#include "Interaction.h"

class HMaterial
{
public:
	HMaterial();
	~HMaterial();

	virtual void ComputeScatterFunction(SurfaceInteraction* si) = 0;

private:

};

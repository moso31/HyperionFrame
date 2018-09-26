#include "Interaction.h"
#include "Shape.h"

SurfaceInteraction::SurfaceInteraction()
{
}

SurfaceInteraction::~SurfaceInteraction()
{
}

void SurfaceInteraction::ComputeScatterFunctions()
{
	HMaterial* material = shape->GetMaterial();
	material->ComputeScatterFunction(this);
}

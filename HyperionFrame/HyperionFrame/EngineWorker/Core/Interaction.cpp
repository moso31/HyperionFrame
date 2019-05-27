#include "Interaction.h"
#include "HShape.h"
#include "HMaterial.h"
#include "Reflection.h"

SurfaceInteraction::SurfaceInteraction(const HFloat3 & p, const HFloat2 & uv, const HFloat3 & wo, const HFloat3 & dpdu, const HFloat3 & dpdv, HShape * shape) :
	uv(uv),
	dpdu(dpdu),
	dpdv(dpdv),
	shape(shape),
	bsdf(nullptr)
{
	this->p = p;
	this->n = dpdv.Cross(dpdu).Normalize();
	this->wo = wo;
}

void SurfaceInteraction::ComputeScatterFunctions()
{
	auto material = shape->GetMaterial();
	if (material)
		material->ComputeScatterFunction(this);
}

Ray Interaction::SpawnRay(const HFloat3 & d) const
{
	//OffsetRayOrigin(p, n, d);
	return Ray(p, d);
}

Ray Interaction::SpawnRayTo(const HFloat3 & p1) const
{
	return Ray(p, p1 - p);
}

Ray Interaction::SpawnRayTo(const Interaction & it) const
{
	return Ray(p, it.p - p);
}

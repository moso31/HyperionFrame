#include "Interaction.h"
#include "HShape.h"
#include "HPBRMaterial.h"
#include "Reflection.h"

SurfaceInteraction::SurfaceInteraction(const HFloat3& p, const HFloat3& pError, const HFloat2& uv, const HFloat3& wo, const HFloat3& dpdu, const HFloat3& dpdv, HShape* shape) :
	Interaction(p, pError, dpdv.Cross(dpdu).Normalize(), wo),
	uv(uv),
	dpdu(dpdu),
	dpdv(dpdv),
	shape(shape),
	bsdf(nullptr)
{
}

void SurfaceInteraction::ComputeScatterFunctions()
{
	auto material = shape->GetPBRMaterial();
	if (material)
		material->ComputeScatterFunction(this);
}

Ray Interaction::SpawnRay(const HFloat3 & d) const
{
	HFloat3 orig = OffsetRayOrigin(p, pError, n, d);
	return Ray(orig, d);
}

Ray Interaction::SpawnRayTo(const HFloat3 & p1) const
{
	HFloat3 dir = p1 - p;
	HFloat3 orig = OffsetRayOrigin(p, pError, n, dir);
	return Ray(orig, dir);
}

Ray Interaction::SpawnRayTo(const Interaction & it) const
{
	HFloat3 dir = it.p - p;
	HFloat3 orig = OffsetRayOrigin(p, pError, n, dir);
	return Ray(orig, dir);
}

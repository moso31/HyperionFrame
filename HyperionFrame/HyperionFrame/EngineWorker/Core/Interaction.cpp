#include "Interaction.h"
#include "HShape.h"
#include "HMaterial.h"
#include "Reflection.h"

SurfaceInteraction::SurfaceInteraction()
{
}

SurfaceInteraction::SurfaceInteraction(const XMFLOAT3 & p, const XMFLOAT2 & uv, const XMFLOAT3 & wo, const XMFLOAT3 & dpdu, const XMFLOAT3 & dpdv, HShape * shape) :
	uv(uv),
	dpdu(dpdu),
	dpdv(dpdv),
	shape(shape)
{
	XMVECTOR vn = XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&dpdv), XMLoadFloat3(&dpdu)));
	XMStoreFloat3(&n, vn);

	this->p = p;
	this->n = n;
	this->wo = wo;
}

SurfaceInteraction::~SurfaceInteraction()
{
}

void SurfaceInteraction::ComputeScatterFunctions()
{
	auto material = shape->GetMaterial();
	if (material)
		material->ComputeScatterFunction(this);
}

Ray Interaction::SpawnRay(const XMFLOAT3 & d) const
{
	//OffsetRayOrigin(p, n, d);
	return Ray(p, d);
}

Ray Interaction::SpawnRayTo(const XMFLOAT3 & p1) const
{
	XMFLOAT3 d;
	XMStoreFloat3(&d, XMLoadFloat3(&p1) - XMLoadFloat3(&p));
	return Ray(p, d);
}

Ray Interaction::SpawnRayTo(const Interaction & it) const
{
	XMFLOAT3 d;
	XMStoreFloat3(&d, XMLoadFloat3(&it.p) - XMLoadFloat3(&p));
	return Ray(p, d);
}

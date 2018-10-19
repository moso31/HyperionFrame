#include "Interaction.h"
#include "Shape.h"

SurfaceInteraction::SurfaceInteraction()
{
}

SurfaceInteraction::SurfaceInteraction(const XMFLOAT3 & p, const XMFLOAT2 & uv, const XMFLOAT3 & wo, const XMFLOAT3 & dpdu, const XMFLOAT3 & dpdv, Shape * shape) :
	uv(uv),
	dpdu(dpdu),
	dpdv(dpdv),
	shape(shape)
{
	XMVECTOR vn = XMVector3Cross(XMLoadFloat3(&dpdv), XMLoadFloat3(&dpdu));
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
	HMaterial* material = shape->GetMaterial();
	if (material)
		material->ComputeScatterFunction(this);
}

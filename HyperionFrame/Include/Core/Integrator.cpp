#include "whitted.h"
#include "Interaction.h"

XMCOLOR3 Integrator::SpecularReflect(const Ray & ray, const SurfaceInteraction & isect, const HScene & scene, int depth)
{
	XMFLOAT3 wo = isect.wo, wi;
	const XMFLOAT3 &p = isect.p;
	const XMFLOAT3 &ns = isect.n;	// 现阶段没有微表面，不考虑isect.shading
	const BSDF &bsdf = *isect.bsdf;

	XMCOLOR3 f = bsdf.Sample_f(wo, &wi, sampler.Get2D(), &pdf, BxDFType(BSDF_REFLECTION | BSDF_SPECULAR));

	XMVECTOR fV = XMLoadFloat3(&f);
	XMVECTOR wiV = XMLoadFloat3(&wi);
	XMVECTOR nsV = XMLoadFloat3(&ns);

	XMCOLOR3 L(0.0f, 0.0f, 0.0f);
	if (!f.IsBlack() && XMVectorGetX(XMVectorAbs(XMVector3Dot(wiV, nsV))) > H_EPSILON)
	{
		Ray ray = isect.SpawnRay(wi);
		L = f * Li(ray, scene, depth + 1) * AbsDot(wi, ns) / pdf;
	}

	return L;
}

XMCOLOR3 Integrator::SpecularTransmit(const Ray & ray, const SurfaceInteraction & isect, const HScene & scene, int depth)
{
	return XMCOLOR3();
}

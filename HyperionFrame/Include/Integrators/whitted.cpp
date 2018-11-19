#include "whitted.h"
#include "HScene.h"
#include "Interaction.h"
#include "Reflection.h"

XMCOLOR3 WhittedIntegrator::Li(const Ray& ray, HSampler& sampler, const HScene& scene, int depth)
{
	bool isHit = false;
	int shapeCount = (int)scene.shapes.size();

	SurfaceInteraction isect;
	int hitShapeIndex = -1;
	if (scene.Intersect(ray, &isect, &hitShapeIndex))
	{
		//printf("hit: name: %s, ID: %d\n", scene.shapes[hitShapeIndex]->GetName().data(), hitShapeIndex);
		XMFLOAT3 wo = isect.wo;

		isect.ComputeScatterFunctions();

		XMCOLORV LV = XMVectorZero();
		for (UINT j = 0; j < scene.lights.size(); j++)
		{
			XMFLOAT3 wi;
			VisibilityTester vis;
			XMCOLOR3 Li = scene.lights[j]->Sample_Li(isect, wi, &vis);
			XMCOLOR3 f = isect.bsdf->f(wo, wi);

			XMCOLORV fV = XMLoadFloat3(&f);
			if (!XMVector3Equal(fV, XMVectorZero()/* && vis.Unoccluded(scene)*/))
			{
				XMCOLORV LiV = XMLoadFloat3(&Li);
				XMVECTOR wiV = XMLoadFloat3(&wi);
				XMVECTOR nV = XMLoadFloat3(&isect.n);

				LV += fV * LiV * XMVectorAbs(XMVector3Dot(wiV, nV));
			}
		}

		if (depth + 1 < maxDepth)
		{
			XMCOLORV fRV = XMLoadFloat3(&SpecularReflect(ray, isect, scene, sampler, depth));
			XMCOLORV fTV = XMLoadFloat3(&SpecularTransmit(ray, isect, scene, sampler, depth));
			LV += fRV + fTV;
		}

		XMCOLOR3 L;
		XMStoreFloat3(&L, LV);
		//printf("R: %f, G: %f, B: %f\n", L.x, L.y, L.z);
		return L;
	}
	return XMCOLOR3(0.0f, 0.0f, 0.0f);
}

XMCOLOR3 WhittedIntegrator::SpecularReflect(const Ray & ray, const SurfaceInteraction & isect, const HScene & scene, HSampler& sampler, int depth)
{
	XMFLOAT3 wo = isect.wo, wi;
	const XMFLOAT3 &p = isect.p;
	const XMFLOAT3 &ns = isect.n;	// 现阶段没有微表面，不考虑isect.shading
	BSDF &bsdf = *isect.bsdf;

	float pdf;
	XMCOLOR3 f = bsdf.Sample_f(wo, &wi, sampler.Get2D(), &pdf, BxDFType(BSDF_REFLECTION | BSDF_SPECULAR));

	XMVECTOR fV = XMLoadFloat3(&f);
	XMVECTOR wiV = XMLoadFloat3(&wi);
	XMVECTOR nsV = XMLoadFloat3(&ns);

	XMCOLOR3 L(0.0f, 0.0f, 0.0f);
	if (!XMVector3Equal(fV, XMVectorZero()) && XMVectorGetX(XMVectorAbs(XMVector3Dot(wiV, nsV))) > H_EPSILON)
	{
		Ray ray = isect.SpawnRay(wi);
		XMVECTOR absdotV = XMVectorAbs(XMVector3Dot(wiV, nsV));
		XMVECTOR LiTempV = XMLoadFloat3(&Li(ray, sampler, scene, depth + 1));
		XMStoreFloat3(&L, fV * LiTempV * absdotV);// / pdf);
	}

	return L;
}

XMCOLOR3 WhittedIntegrator::SpecularTransmit(const Ray & ray, const SurfaceInteraction & isect, const HScene & scene, HSampler& sampler, int depth)
{
	XMFLOAT3 wo = isect.wo, wi;
	const XMFLOAT3 &p = isect.p;
	const XMFLOAT3 &ns = isect.n;	// 现阶段没有微表面，不考虑isect.shading
	BSDF &bsdf = *isect.bsdf;

	float pdf;
	XMCOLOR3 f = bsdf.Sample_f(wo, &wi, sampler.Get2D(), &pdf, BxDFType(BSDF_TRANSMISSION | BSDF_SPECULAR));

	XMVECTOR fV = XMLoadFloat3(&f);
	XMVECTOR wiV = XMLoadFloat3(&wi);
	XMVECTOR nsV = XMLoadFloat3(&ns);

	XMCOLOR3 L(0.0f, 0.0f, 0.0f);
	if (!XMVector3Equal(fV, XMVectorZero()) && XMVectorGetX(XMVectorAbs(XMVector3Dot(wiV, nsV))) > H_EPSILON)
	{
		Ray ray = isect.SpawnRay(wi);
		XMVECTOR absdotV = XMVectorAbs(XMVector3Dot(wiV, nsV));
		XMVECTOR LiTempV = XMLoadFloat3(&Li(ray, sampler, scene, depth + 1));
		XMStoreFloat3(&L, fV * LiTempV * absdotV);// / pdf);
	}

	return L;
}

#include "whitted.h"
#include "HScene.h"
#include "Interaction.h"
#include "Reflection.h"

HFloat3 WhittedIntegrator::Li(const Ray& ray, HSampler& sampler, const HScene& scene, HInt depth, vector<Segment>* out_debug_rayTraceData)
{
	bool isHit = false;

	SurfaceInteraction isect;
	HInt hitShapeIndex = -1;
	if (scene.Intersect(ray, &isect, &hitShapeIndex))
	{
#if _DEBUG
		Ray r;
		HFloat3 ro, rd;
		r = ray;
		ro = r.origin;
		rd = r.direction;
		printf("Ray: orig: %.3f, %.3f, %.3f;  dir: %.3f, %.3f, %.3f\n", ro.x, ro.y, ro.z, rd.x, rd.y, rd.z);

		printf("hit: name: %s, ID: %d\n", scene.primitives[hitShapeIndex]->GetName().data(), hitShapeIndex);

		Segment seg(ro, isect.p);

		if (out_debug_rayTraceData)
			out_debug_rayTraceData->push_back(seg);
#endif
		HFloat3 wo = isect.wo;

		isect.ComputeScatterFunctions();

		HFloat3 L(0.0f);
		for (HUInt j = 0; j < scene.lights.size(); j++)
		{
			HFloat3 wi;
			VisibilityTester vis;
			HFloat3 Li = scene.lights[j]->Sample_Li(isect, wi, &vis);

			HFloat3 f = isect.bsdf->f(wo, wi);
			if (f != 0.0 && vis.Unoccluded(scene))
			{
				L += f * Li * fabsf(wi.Dot(isect.n));
			}
		}

		if (depth + 1 < maxDepth)
		{
#if _DEBUG
			printf("R %d...\n", depth);
#endif
			HFloat3 fR = SpecularReflect(ray, isect, scene, sampler, depth, out_debug_rayTraceData);
#if _DEBUG
			printf("T %d...\n", depth);
#endif
			HFloat3 fT = SpecularTransmit(ray, isect, scene, sampler, depth, out_debug_rayTraceData);
			L += fR + fT;
		}

		//printf("R: %f, G: %f, B: %f\n", L.x, L.y, L.z);
		return L;
	}
	return HFloat3(0.0f, 0.0f, 0.0f);
}

HFloat3 WhittedIntegrator::SpecularReflect(const Ray & ray, const SurfaceInteraction & isect, const HScene & scene, HSampler& sampler, HInt depth, vector<Segment>* out_debug_rayTraceData)
{
	HFloat3 wo = isect.wo, wi;
	const HFloat3 &p = isect.p;
	const HFloat3 &ns = isect.n;	// 现阶段没有微表面，不考虑isect.shading
	BSDF &bsdf = *isect.bsdf;

	HFloat pdf;
	HFloat3 f = bsdf.Sample_f(wo, &wi, sampler.Get2D(), &pdf, BxDFType(BSDF_REFLECTION | BSDF_SPECULAR));

	HFloat3 L(0.0f, 0.0f, 0.0f);
	if (f != 0.0 && fabsf(wi.Dot(ns)) > H_EPSILON)
	{
		Ray ray = isect.SpawnRay(wi);
		HFloat3 LiTemp = Li(ray, sampler, scene, depth + 1, out_debug_rayTraceData);
		L = f * LiTemp * fabsf(wi.Dot(ns));// / pdf);
	}

	return L;
}

HFloat3 WhittedIntegrator::SpecularTransmit(const Ray & ray, const SurfaceInteraction & isect, const HScene & scene, HSampler& sampler, HInt depth, vector<Segment>* out_debug_rayTraceData)
{
	HFloat3 wo = isect.wo, wi;
	const HFloat3 &p = isect.p;
	const HFloat3 &ns = isect.n;	// 现阶段没有微表面，不考虑isect.shading
	BSDF &bsdf = *isect.bsdf;

	HFloat pdf;
	HFloat3 f = bsdf.Sample_f(wo, &wi, sampler.Get2D(), &pdf, BxDFType(BSDF_TRANSMISSION | BSDF_SPECULAR));

	HFloat3 L(0.0f, 0.0f, 0.0f);
	if (f != 0.0 && fabsf(wi.Dot(ns)) > H_EPSILON)
	{
		Ray ray = isect.SpawnRay(wi);
		HFloat3 LiTemp = Li(ray, sampler, scene, depth + 1, out_debug_rayTraceData);
		L = f * LiTemp * fabsf(wi.Dot(ns));// / pdf);
	}

	return L;
}

#include "whitted.h"
#include "Interaction.h"

XMCOLOR3 WhittedIntegrator::Li(const Ray& ray, const HScene& scene, int depth)
{
	bool isHit = false;
	int shapeCount = (int)scene.shapes.size();
	for (int i = 0; i < shapeCount; i++)
	{
		auto shapeList = scene.shapes;
		if (shapeList[i]->IntersectP(ray))
		{
			SurfaceInteraction isect;
			//printf("Object %d intersected.\n", i);
			int index;
			shapeList[i]->Intersect(ray, index, &isect);
			//printf("hit: %f, %f, %f\n", isect.p.x, isect.p.y, isect.p.z);
			if (index == -1)
				continue;

			XMFLOAT3 wo = isect.wo;

			isect.ComputeScatterFunctions();

			XMCOLORV LV = XMVectorZero();
			for (UINT j = 0; j < scene.lights.size(); j++)
			{
				XMFLOAT3 wi;
				VisibilityTester vis;
				XMCOLOR3 Li = scene.lights[j]->Sample_Li(isect, wi, &vis);
				XMCOLOR3 f = isect.bsdf->f(wo, wi);

				//if (/*!f.IsBlack() && */vis.Unoccluded(scene))
				{
					XMCOLORV fV = XMLoadFloat3(&f);
					XMCOLORV LiV = XMLoadFloat3(&Li);
					XMVECTOR wiV = XMLoadFloat3(&wi);
					XMVECTOR nV = XMLoadFloat3(&isect.n);

					LV += fV * LiV * XMVectorAbs(XMVector3Dot(wiV, nV));
				}
			}

			if (depth + 1 < maxDepth)
			{
				LV += SpecularReflect(ray, isect, scene, sampler, arena, depth);
				LV += SpecularTransmit(ray, isect, scene, sampler, arena, depth);
			}

			XMCOLOR3 L;
			XMStoreFloat3(&L, LV);
			//printf("R: %f, G: %f, B: %f\n", L.x, L.y, L.z);
			return L;
		}
	}
	return XMCOLOR3(0.0f, 0.0f, 0.0f);
}

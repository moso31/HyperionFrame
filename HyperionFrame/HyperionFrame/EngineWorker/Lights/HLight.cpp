#include "HLight.h"
#include "HScene.h"

HLight::HLight()
{
}

HLight::~HLight()
{
}

bool VisibilityTester::Unoccluded(const HScene & scene) const
{
	int ignore;
	float tMax = XMVectorGetX(XMVector3Length(XMLoadFloat3(&p1.p) - XMLoadFloat3(&p0.p)));
	return !scene.Intersect(p0.SpawnRayTo(p1), &SurfaceInteraction(), &ignore, tMax);
}

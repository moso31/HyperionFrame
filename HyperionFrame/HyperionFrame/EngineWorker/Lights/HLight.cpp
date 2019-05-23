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
	HInt ignore;
	HFloat tMax = (p1.p - p0.p).Length();
	return !scene.Intersect(p0.SpawnRayTo(p1), &SurfaceInteraction(), &ignore, tMax);
}

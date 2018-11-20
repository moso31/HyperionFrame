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
	return !scene.Intersect(p0.SpawnRayTo(p1), &SurfaceInteraction(), &ignore);
}

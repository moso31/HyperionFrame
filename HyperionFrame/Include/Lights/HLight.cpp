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
	//return !scene.Intersect(p0.SpawnRayTo(p1));
	return false;
}

#pragma once
#include "Transform.h"
#include "Interaction.h"

class HLight
{
public:
	HLight();
	~HLight();

	virtual XMFLOAT3 Sample_Li(const SurfaceInteraction& isect, XMFLOAT3& out_wi, VisibilityTester* out_vis) = 0;

private:

};

class VisibilityTester
{
public:
	VisibilityTester() {};
	VisibilityTester(const SurfaceInteraction& p0, const SurfaceInteraction& p1) :
		p0(p0), p1(p1) {};
	//bool Unoccluded(const Scene &scene) const;

public:
	SurfaceInteraction p0, p1;
};
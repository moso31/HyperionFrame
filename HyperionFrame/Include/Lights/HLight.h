#pragma once
#include "Transform.h"
#include "Interaction.h"

class HLight : public Transform
{
public:
	HLight();
	~HLight();

	virtual XMCOLOR3 Sample_Li(const Interaction& isect, XMFLOAT3& out_wi, VisibilityTester* out_vis) = 0;

private:

};

class VisibilityTester
{
public:
	VisibilityTester() {};
	VisibilityTester(const Interaction& p0, const Interaction& p1) :
		p0(p0), p1(p1) {};
	//bool Unoccluded(const Scene &scene) const;

public:
	Interaction p0, p1;
};
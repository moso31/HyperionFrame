#pragma once
#include "HMaterial.h"

class HMirrorMaterial : public HMaterial
{
public:
	HMirrorMaterial();
	HMirrorMaterial(const XMCOLOR3& _Kr);
	~HMirrorMaterial();

	void ComputeScatterFunction(SurfaceInteraction* si);

private:
	XMCOLOR3 Kr;
};

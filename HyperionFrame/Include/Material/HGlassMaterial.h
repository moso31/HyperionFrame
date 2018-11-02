#pragma once
#include "HMaterial.h"

class HGlassMaterial : public HMaterial
{
public:
	HGlassMaterial();
	HGlassMaterial(const XMCOLOR3 & _Kr, const XMCOLOR3 & _Kt, float _eta);
	~HGlassMaterial();

	void ComputeScatterFunction(SurfaceInteraction* si);

private:
	XMCOLOR3 Kr, Kt;
	float eta;
};

#include "HMirrorMaterial.h"

HMirrorMaterial::HMirrorMaterial()
{
}

HMirrorMaterial::HMirrorMaterial(const HFloat3 & _Kr) : 
	HMaterial(HMAT_MIRROR)
{
	Kr = _Kr;
}

HMirrorMaterial::~HMirrorMaterial()
{
}

void HMirrorMaterial::ComputeScatterFunction(SurfaceInteraction * si)
{
	si->bsdf = new BSDF(*si);
	HFloat3 r = Clamp(Kr, 0.0f, 1.0f); /* = Kd->Evaluate(*si).Clamp();*/
	if (r != 0.0f)
	{
		si->bsdf->Add(new SpecularReflection(r, new FresnelNoOp()));
	}
}

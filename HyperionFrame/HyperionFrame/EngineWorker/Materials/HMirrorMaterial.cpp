#include "HMirrorMaterial.h"

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
	HFloat3 r = Kr.Clamp(0.0f, 1.0f);	//Kr->Evaluate(*si).Clamp();
	if (r != 0.0f)
	{
		si->bsdf->Add(new SpecularReflection(r, new FresnelNoOp()));
	}
}

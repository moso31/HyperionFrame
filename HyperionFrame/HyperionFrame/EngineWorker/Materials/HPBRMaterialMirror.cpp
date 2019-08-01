#include "HPBRMaterialMirror.h"

HPBRMaterialMirror::HPBRMaterialMirror(const HFloat3 & _Kr) : 
	HPBRMaterial(HMAT_MIRROR)
{
	Kr = _Kr;
}

HPBRMaterialMirror::~HPBRMaterialMirror()
{
}

void HPBRMaterialMirror::ComputeScatterFunction(SurfaceInteraction * si)
{
	si->bsdf = new BSDF(*si);
	HFloat3 r = Kr.Clamp(0.0f, 1.0f);	//Kr->Evaluate(*si).Clamp();
	if (r != 0.0f)
	{
		si->bsdf->Add(new SpecularReflection(r, new FresnelNoOp()));
	}
}

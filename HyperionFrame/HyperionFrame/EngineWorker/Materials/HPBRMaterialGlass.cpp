#include "HPBRMaterialGlass.h"

HPBRMaterialGlass::HPBRMaterialGlass(const HFloat3 & _Kr, const HFloat3 & _Kt, HFloat _eta) :
	HPBRMaterial(HMAT_GLASS)
{
	Kr = _Kr;
	Kt = _Kt;
	eta = _eta; 
}

HPBRMaterialGlass::~HPBRMaterialGlass()
{
}

void HPBRMaterialGlass::ComputeScatterFunction(SurfaceInteraction * si)
{
	HFloat3 R = Kr.Clamp(0.0f, 1.0f);
	HFloat3 T = Kt.Clamp(0.0f, 1.0f);

	si->bsdf = new BSDF(*si, eta);

	Fresnel *fresnel = new FresnelDielectric(1.0f, eta);
	if (R != 0.0)
		si->bsdf->Add(new SpecularReflection(R, fresnel));
	if (T != 0.0)
		si->bsdf->Add(new SpecularTransmission(T, 1.0f, eta));
}

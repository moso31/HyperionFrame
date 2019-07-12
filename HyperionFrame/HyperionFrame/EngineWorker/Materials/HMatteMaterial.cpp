#include "HMatteMaterial.h"

HMatteMaterial::HMatteMaterial(const HFloat3 & _Kd, const HFloat & _sigma) :
	HMaterial(HMAT_MATTE)
{
	Kd = _Kd;
	sigma = _sigma;
}

HMatteMaterial::~HMatteMaterial()
{
}

void HMatteMaterial::ComputeScatterFunction(SurfaceInteraction * si)
{
	si->bsdf = new BSDF(*si);
	HFloat3 r = Kd.Clamp(0.0f, 1.0f);	//Kd->Evaluate(*si).Clamp();
	HFloat sig = Clamp(sigma/*->Evaluate(*si)*/, 0.0f, 90.0f);
	if (r != 0.0f)
	{
		//if (sig == 0)
		si->bsdf->Add(new LambertianReflection(r));
		//else
		//	si->bsdf->Add(new OrenNayar(r, sig));
	}
}

#include "HMatteMaterial.h"

HMatteMaterial::HMatteMaterial()
{
}

HMatteMaterial::HMatteMaterial(const XMFLOAT3 & _Kd, const float & _sigma)
{
	Kd = _Kd;
	sigma = _sigma;
}

HMatteMaterial::~HMatteMaterial()
{
}

void HMatteMaterial::ComputeScatterFunction(SurfaceInteraction * si)
{
	si->bsdf = new BSDF(si);
	Spectrum r = Kd->Evaluate(*si).Clamp();
	Float sig = Clamp(sigma->Evaluate(*si), 0, 90);
	if (!r.IsBlack()) {
		if (sig == 0)
			si->bsdf->Add(ARENA_ALLOC(arena, LambertianReflection)(r));
		else
			si->bsdf->Add(ARENA_ALLOC(arena, OrenNayar)(r, sig));
	}
}

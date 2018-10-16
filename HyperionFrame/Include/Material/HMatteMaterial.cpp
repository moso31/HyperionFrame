#include "HMatteMaterial.h"
#include "Reflection.h"

HMatteMaterial::HMatteMaterial()
{
}

HMatteMaterial::HMatteMaterial(const XMCOLOR3 & _Kd, const float & _sigma)
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
	XMCOLOR3 r; /* = Kd->Evaluate(*si).Clamp();*/
	XMStoreFloat3(&r, XMVectorClamp(XMLoadFloat3(&Kd), XMVectorReplicate(0.0f), XMVectorReplicate(90.0f)));
	float sig = Clamp(sigma/*->Evaluate(*si)*/, 0.0f, 90.0f);
	//if (!r.IsBlack()) {
	//	if (sig == 0)
	//		si->bsdf->Add(ARENA_ALLOC(arena, LambertianReflection)(r));
	//	else
	//		si->bsdf->Add(ARENA_ALLOC(arena, OrenNayar)(r, sig));
	//}
	si->bsdf->Add(new LambertianReflection(r));
}

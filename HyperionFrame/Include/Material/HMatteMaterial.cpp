#include "HMatteMaterial.h"

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
	XMCOLORV rV = XMVectorClamp(XMLoadFloat3(&Kd), XMVectorZero(), XMVectorReplicate(1.0f));
	float sig = Clamp(sigma/*->Evaluate(*si)*/, 0.0f, 90.0f);
	if (!XMVector3Equal(rV, XMVectorZero()))
	{
		XMStoreFloat3(&r, rV);

		//if (sig == 0)
		si->bsdf->Add(new LambertianReflection(r));
		//else
		//	si->bsdf->Add(new OrenNayar(r, sig));
	}
}

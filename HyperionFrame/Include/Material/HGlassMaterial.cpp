#include "HGlassMaterial.h"
#include "Reflection.h"

HGlassMaterial::HGlassMaterial()
{
}

HGlassMaterial::HGlassMaterial(const XMCOLOR3 & _Kr, const XMCOLOR3 & _Kt, float _eta)
{
	Kr = _Kr;
	Kt = _Kt;
	eta = _eta; 
}

HGlassMaterial::~HGlassMaterial()
{
}

void HGlassMaterial::ComputeScatterFunction(SurfaceInteraction * si)
{
	XMFLOAT3 R = Kr, T = Kt;
	XMCOLORV RV = XMVectorClamp(XMLoadFloat3(&R), XMVectorZero(), XMVectorReplicate(1.0f));
	XMCOLORV TV = XMVectorClamp(XMLoadFloat3(&T), XMVectorZero(), XMVectorReplicate(1.0f));

	si->bsdf = new BSDF(*si, eta);

	Fresnel *fresnel = new FresnelDielectric(1.0f, eta);
	if (!XMVector3Equal(RV, XMVectorZero()))
		si->bsdf->Add(new SpecularReflection(R, fresnel));
	if (!XMVector3Equal(TV, XMVectorZero()))
		si->bsdf->Add(new SpecularTransmission(T, 1.0f, eta));
}

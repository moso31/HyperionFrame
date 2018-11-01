#include "HMirrorMaterial.h"
#include "Reflection.h"

HMirrorMaterial::HMirrorMaterial()
{
}

HMirrorMaterial::HMirrorMaterial(const XMCOLOR3 & _Kr)
{
	Kr = _Kr;
}

HMirrorMaterial::~HMirrorMaterial()
{
}

void HMirrorMaterial::ComputeScatterFunction(SurfaceInteraction * si)
{
	si->bsdf = new BSDF(*si);
	XMCOLOR3 r; /* = Kd->Evaluate(*si).Clamp();*/
	XMCOLORV rV = XMVectorClamp(XMLoadFloat3(&Kr), XMVectorZero(), XMVectorReplicate(1.0f));
	if (!XMVector3Equal(rV, XMVectorZero()))
	{
		XMStoreFloat3(&r, rV);
		si->bsdf->Add(new SpecularReflection(r, new FresnelNoOp()));
	}
}

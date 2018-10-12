#include "Reflection.h"

BSDF::BSDF(const SurfaceInteraction & si, float eta)
	: n(si.n), s(si.dpdu) 
{
	XMStoreFloat3(&t, XMVector3Cross(XMLoadFloat3(&n), XMLoadFloat3(&s)));
}

XMCOLOR3 BSDF::f(const XMFLOAT3 & woW, const XMFLOAT3 & wiW, BxDFType flags) 
{
	XMFLOAT3 wi = WorldToReflectionCoord(wiW), wo = WorldToReflectionCoord(woW);
	if (wo.z == 0) 
		return { 0.0f, 0.0f, 0.0f };

	XMVECTOR nV = XMLoadFloat3(&n);
	float reflect = XMVectorGetX(XMVector3Dot(XMLoadFloat3(&wiW), nV)) * XMVectorGetX(XMVector3Dot(XMLoadFloat3(&woW), nV));
	bool isReflect = reflect > 0;

	XMCOLOR3 f(0.0f, 0.0f, 0.0f); 
	XMVECTOR fV = XMVectorZero();
	for (UINT i = 0; i < m_bxdfs.size(); ++i)
		if (m_bxdfs[i]->MatchesFlags(flags) &&
			((isReflect && (m_bxdfs[i]->type & BSDF_REFLECTION)) ||
			(!isReflect && (m_bxdfs[i]->type & BSDF_TRANSMISSION))))
		{
			 fV += XMLoadFloat3(&m_bxdfs[i]->f(wo, wi));
		}
	XMStoreFloat3(&f, fV);
	return f;
}

XMFLOAT3 BSDF::WorldToReflectionCoord(const XMFLOAT3 & v)
{
	return XMFLOAT3(
		v.x * s.x + v.y * s.y + v.z * s.z,
		v.x * t.x + v.y * t.y + v.z * t.z,
		v.x * n.x + v.y * n.y + v.z * n.z
	);
}

XMFLOAT3 BSDF::ReflectionToWorldCoord(const XMFLOAT3 & v)
{
	return XMFLOAT3(
		v.x * s.x + v.y * t.x + v.z * n.x,
		v.x * s.y + v.y * t.y + v.z * n.y,
		v.x * s.z + v.y * t.z + v.z * n.z
	);
}

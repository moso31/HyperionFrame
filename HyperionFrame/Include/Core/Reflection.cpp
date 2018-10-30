#include "Reflection.h"

namespace Reflect
{
	float FrDielectric(float cosThetaI, float etaI, float etaT)
	{
		cosThetaI = Clamp(cosThetaI, -1, 1);
		bool entering = cosThetaI > 0.f;
		if (!entering) 
		{
			std::swap(etaI, etaT);
			cosThetaI = std::abs(cosThetaI);
		}

		float sinThetaI = sqrtf(max(0.0f, 1 - cosThetaI * cosThetaI));
		float sinThetaT = etaI / etaT * sinThetaI;

		if (sinThetaT >= 1) return 1;
		float cosThetaT = sqrtf(max(0.0f, 1 - sinThetaT * sinThetaT));

		float Rparl = ((etaT * cosThetaI) - (etaI * cosThetaT)) / ((etaT * cosThetaI) + (etaI * cosThetaT));
		float Rperp = ((etaI * cosThetaI) - (etaT * cosThetaT)) / ((etaI * cosThetaI) + (etaT * cosThetaT));
		return (Rparl * Rparl + Rperp * Rperp) / 2;
	}

	XMCOLOR3 FrConductor(float cosThetaI, const XMCOLOR3 & etaI, const XMCOLOR3 & etaT, const XMCOLOR3 & k)
	{
		XMCOLORV etaIV = XMLoadFloat3(&etaI);
		XMCOLORV etaTV = XMLoadFloat3(&etaT);
		XMCOLORV kV = XMLoadFloat3(&k);

		cosThetaI = Clamp(cosThetaI, -1, 1);
		XMCOLORV etaV = etaTV / etaIV;
		XMCOLORV etak = kV / etaIV;

		float cosThetaI2 = cosThetaI * cosThetaI;
		float sinThetaI2 = 1.0f - cosThetaI2;

		XMCOLORV cosThetaI2V = XMVectorReplicate(cosThetaI2);
		XMCOLORV sinThetaI2V = XMVectorReplicate(sinThetaI2);

		XMCOLORV eta2V = etaV * etaV;
		XMCOLORV etak2V = etak * etak;

		XMCOLORV t0V = eta2V - etak2V - (sinThetaI2V);
		XMCOLORV a2b2V = XMVectorSqrt(t0V * t0V + 4 * eta2V * etak2V);
		XMCOLORV t1V = a2b2V + cosThetaI2V;
		XMCOLORV aV = XMVectorSqrt(0.5f * (a2b2V + t0V));
		XMCOLORV t2V = 2.0f * cosThetaI * aV;
		XMCOLORV RsV = (t1V - t2V) / (t1V + t2V);

		XMCOLORV t3V = cosThetaI2V * a2b2V + XMVectorReplicate(sinThetaI2 * sinThetaI2);
		XMCOLORV t4V = t2V * sinThetaI2;
		XMCOLORV RpV = RsV * (t3V - t4V) / (t3V + t4V);

		XMCOLOR3 result;
		XMStoreFloat3(&result, 0.5f * (RpV + RsV));
		return result;
	}
}

using namespace Reflect;

BSDF::BSDF(const SurfaceInteraction & si, float eta)
	: n(si.n), s(si.dpdu) 
{
	XMStoreFloat3(&t, XMVector3Cross(XMLoadFloat3(&n), XMLoadFloat3(&s)));
}

void BSDF::Add(BxDF * bxdf)
{
	m_bxdfs.push_back(bxdf);
}

int BSDF::NumComponents(BxDFType type)
{
	int num = 0;
	for (int i = 0; i < m_bxdfs.size(); ++i)
		if (m_bxdfs[i]->MatchesFlags(type)) ++num;
	return num;
}

XMCOLOR3 BSDF::f(const XMFLOAT3 & woW, const XMFLOAT3 & wiW, BxDFType flags)
{
	XMFLOAT3 wi = WorldToReflectionCoord(wiW), wo = WorldToReflectionCoord(woW);
	//printf("wi: %f, %f, %f   wo: %f, %f, %f\n", wiW.x, wiW.y, wiW.z, woW.x, woW.y, woW.z);
	//printf("wi: %f, %f, %f   wo: %f, %f, %f\n", wi.x, wi.y, wi.z, wo.x, wo.y, wo.z);
	if (wo.z == 0)
		return { 0.0f, 0.0f, 0.0f };

	XMVECTOR nV = XMLoadFloat3(&n);
	float reflect = XMVectorGetX(XMVector3Dot(XMLoadFloat3(&wiW), nV)) * XMVectorGetX(XMVector3Dot(XMLoadFloat3(&woW), nV));
	bool isReflect = reflect > 0;
	//printf("normal: %f, %f, %f, reflect: %f\n", n.x, n.y, n.z, reflect);

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

XMCOLOR3 BSDF::Sample_f(const XMFLOAT3 & woW, XMFLOAT3 * wiW, const XMFLOAT2 & u, float * pdf, BxDFType type/*, BxDFType *sampledType*/)
{
	int matchingComps = NumComponents(type);
	if (matchingComps == 0) {
		*pdf = 0;
		//if (sampledType) *sampledType = BxDFType(0);
		return { 0.0f, 0.0f, 0.0f };
	}

	int comp = min((int)floorf(u.x * matchingComps), matchingComps - 1);
	BxDF *bxdf = nullptr;
	int count = comp;
	for (int i = 0; i < m_bxdfs.size(); ++i)
	{
		if (m_bxdfs[i]->MatchesFlags(type) && count-- == 0)
		{
			bxdf = m_bxdfs[i];
			break;
		}
	}

	XMFLOAT2 uRemapped(min(u.x * matchingComps - comp, ONE_MINUS_EPSILON), u.y);

	XMFLOAT3 wi, wo = WorldToReflectionCoord(woW);
	if (wo.z == 0) return { 0.0f, 0.0f, 0.0f };
	*pdf = 0.f;
	//if (sampledType) *sampledType = bxdf->type;
	XMCOLOR3 f = bxdf->Sample_f(wo, &wi, uRemapped, pdf/*, sampledType*/);

	if (*pdf == 0) 
	{
		//if (sampledType) *sampledType = BxDFType(0); 
		return { 0.0f, 0.0f, 0.0f };
	}
	*wiW = ReflectionToWorldCoord(wi);

	//if (!(bxdf->type & BSDF_SPECULAR) && matchingComps > 1)
	//	for (int i = 0; i < nBxDFs; ++i)
	//		if (bxdfs[i] != bxdf && bxdfs[i]->MatchesFlags(type))
	//			*pdf += bxdfs[i]->Pdf(wo, wi);
	//if (matchingComps > 1) *pdf /= matchingComps;

	XMVECTOR woWV = XMLoadFloat3(&woW);
	XMVECTOR wiWV = XMLoadFloat3(wiW);
	XMVECTOR ngV = XMLoadFloat3(&n);

	if (!(bxdf->type & BSDF_SPECULAR)) 
	{
		bool reflect = XMVectorGetX(XMVector3Dot(wiWV, ngV) * XMVector3Dot(woWV, ngV)) > 0;
		XMVECTOR fV = XMVectorZero();
		for (int i = 0; i < m_bxdfs.size(); ++i)
			if (m_bxdfs[i]->MatchesFlags(type) &&
				((reflect && (m_bxdfs[i]->type & BSDF_REFLECTION)) ||
				(!reflect && (m_bxdfs[i]->type & BSDF_TRANSMISSION))))
				fV += XMLoadFloat3(&m_bxdfs[i]->f(wo, wi));

		XMStoreFloat3(&f, fV);
	}

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

XMCOLOR3 BxDF::Sample_f(const XMFLOAT3 & wo, XMFLOAT3 * wi, const XMFLOAT2 & u, float * pdf) const
{
	*wi = CosineSampleHemisphere(u);
	if (wo.z < 0) wi->z *= -1;
	//*pdf = Pdf(wo, *wi);
	return f(wo, *wi);
}

XMCOLOR3 FresnelConductor::Evaluate(float cosThetaI) const
{
	return FrConductor(cosThetaI, etaI, etaT, k);
}

XMCOLOR3 FresnelDielectric::Evaluate(float cosThetaI) const
{
	float val = FrDielectric(cosThetaI, etaI, etaT);
	return XMCOLOR3(val, val, val);
}

XMCOLOR3 LambertianReflection::f(const XMFLOAT3 & wo, const XMFLOAT3 & wi) const
{
	return XMCOLOR3(R.x * H_1DIVPI, R.y * H_1DIVPI, R.z * H_1DIVPI);
}

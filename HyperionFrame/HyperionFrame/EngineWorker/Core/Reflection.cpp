#include "Reflection.h"
#include "Interaction.h"

namespace Reflection
{
	HFloat FrDielectric(HFloat cosThetaI, HFloat etaI, HFloat etaT)
	{
		cosThetaI = Clamp(cosThetaI, -1, 1);
		bool entering = cosThetaI > 0.f;
		if (!entering) 
		{
			std::swap(etaI, etaT);
			cosThetaI = std::abs(cosThetaI);
		}

		HFloat sinThetaI = sqrtf(max(0.0f, 1.0f - cosThetaI * cosThetaI));
		HFloat sinThetaT = etaI / etaT * sinThetaI;

		if (sinThetaT >= 1.0f) return 1.0f;
		HFloat cosThetaT = sqrtf(max(0.0f, 1.0f - sinThetaT * sinThetaT));

		HFloat Rparl = ((etaT * cosThetaI) - (etaI * cosThetaT)) / ((etaT * cosThetaI) + (etaI * cosThetaT));
		HFloat Rperp = ((etaI * cosThetaI) - (etaT * cosThetaT)) / ((etaI * cosThetaI) + (etaT * cosThetaT));
		return (Rparl * Rparl + Rperp * Rperp) / 2.0f;
	}

	HFloat3 FrConductor(HFloat cosThetaI, const HFloat3 & etaI, const HFloat3 & etaT, const HFloat3 & k)
	{
		cosThetaI = Clamp(cosThetaI, -1, 1);
		HFloat3 eta = etaT / etaI;
		HFloat3 etak = k / etaI;

		HFloat cosThetaI2 = cosThetaI * cosThetaI;
		HFloat sinThetaI2 = 1.0f - cosThetaI2;

		HFloat3 eta2 = eta * eta;
		HFloat3 etak2 = etak * etak;

		HFloat3 t0 = eta2 - etak2 - (sinThetaI2);
		HFloat3 a2b2 = (t0 * t0 + 4 * eta2 * etak2).Sqrt();
		HFloat3 t1 = a2b2 + cosThetaI2;
		HFloat3 a = (0.5f * (a2b2 + t0)).Sqrt();
		HFloat3 t2 = 2.0f * cosThetaI * a;
		HFloat3 Rs = (t1 - t2) / (t1 + t2);

		HFloat3 t3 = cosThetaI2 * a2b2 + sinThetaI2 * sinThetaI2;
		HFloat3 t4 = t2 * sinThetaI2;
		HFloat3 Rp = Rs * (t3 - t4) / (t3 + t4);

		return 0.5f * (Rp + Rs);
	}

	HFloat CosTheta(const HFloat3 & w)
	{
		return w.z;
	}

	HFloat AbsCosTheta(const HFloat3 & w)
	{
		return abs(w.z);
	}

	HFloat3 Reflect(const HFloat3 &wo, const HFloat3 &n) 
	{
		return -wo + 2.0f * wo.Dot(n) * n;
	}

	bool Refract(const HFloat3 &wi, const HFloat3 &n, HFloat eta, HFloat3 *out_wt)
	{
		HFloat cosThetaI = n.Dot(wi);
		HFloat sin2ThetaI = max(0.0f, 1.0f - cosThetaI * cosThetaI);
		HFloat sin2ThetaT = (eta * eta) * sin2ThetaI;

		if (sin2ThetaT >= 1.0f) return false;

		HFloat cosThetaT = sqrtf(1 - sin2ThetaT);

		*out_wt = eta * -wi + (eta * cosThetaI - cosThetaT) * n;
		return true;
	}

	HFloat3 Faceforward(const HFloat3 &n, const HFloat3 &v) 
	{
		return (n.Dot(v) < 0.0f ? -n : n);
	}
}

using namespace Reflection;

BSDF::BSDF(const SurfaceInteraction & si, HFloat eta)
	: n(si.n), s(si.dpdu) 
{
	t = n.Cross(s);
}

void BSDF::Add(BxDF * bxdf)
{
	m_bxdfs.push_back(bxdf);
}

HInt BSDF::NumComponents(BxDFType type)
{
	HInt num = 0;
	for (size_t i = 0; i < m_bxdfs.size(); ++i)
		if (m_bxdfs[i]->MatchesFlags(type)) ++num;
	return num;
}

HFloat3 BSDF::f(const HFloat3 & woW, const HFloat3 & wiW, BxDFType flags)
{
	HFloat3 wi = WorldToReflectionCoord(wiW), wo = WorldToReflectionCoord(woW);
	//printf("wi: %f, %f, %f   wo: %f, %f, %f\n", wiW.x, wiW.y, wiW.z, woW.x, woW.y, woW.z);
	if (wo.z == 0)
		return { 0.0f, 0.0f, 0.0f };

	HFloat reflect = wiW.Dot(n) * woW.Dot(n);
	bool isReflect = reflect > 0;
	//printf("normal: %f, %f, %f, reflect: %f\n", n.x, n.y, n.z, reflect);

	HFloat3 f(0.0f); 
	for (HUInt i = 0; i < m_bxdfs.size(); ++i)
		if (m_bxdfs[i]->MatchesFlags(flags) &&
			((isReflect && (m_bxdfs[i]->type & BSDF_REFLECTION)) ||
			(!isReflect && (m_bxdfs[i]->type & BSDF_TRANSMISSION))))
		{
			 f += m_bxdfs[i]->f(wo, wi);
		}
	return f;
}

HFloat3 BSDF::Sample_f(const HFloat3 & woW, HFloat3 * wiW, const HFloat2 & u, HFloat * pdf, BxDFType type/*, BxDFType *sampledType*/)
{
	HInt matchingComps = NumComponents(type);
	if (matchingComps == 0) {
		*pdf = 0;
		//if (sampledType) *sampledType = BxDFType(0);
		return { 0.0f, 0.0f, 0.0f };
	}

	HInt comp = min((HInt)floorf(u.x * matchingComps), matchingComps - 1);
	BxDF *bxdf = nullptr;
	HInt count = comp;
	for (size_t i = 0; i < m_bxdfs.size(); ++i)
	{
		if (m_bxdfs[i]->MatchesFlags(type) && count-- == 0)
		{
			bxdf = m_bxdfs[i];
			break;
		}
	}

	HFloat2 uRemapped(min(u.x * matchingComps - comp, H_ONE_MINUS_EPSILON), u.y);

	HFloat3 wi, wo = WorldToReflectionCoord(woW);
	if (wo.z == 0) return { 0.0f, 0.0f, 0.0f };
	*pdf = 0.f;
	//if (sampledType) *sampledType = bxdf->type;
	HFloat3 f = bxdf->Sample_f(wo, &wi, uRemapped, pdf/*, sampledType*/);

	if (*pdf == 0) 
	{
		//if (sampledType) *sampledType = BxDFType(0); 
		return { 0.0f, 0.0f, 0.0f };
	}
	*wiW = ReflectionToWorldCoord(wi);

	//if (!(bxdf->type & BSDF_SPECULAR) && matchingComps > 1)
	//	for (HInt i = 0; i < nBxDFs; ++i)
	//		if (bxdfs[i] != bxdf && bxdfs[i]->MatchesFlags(type))
	//			*pdf += bxdfs[i]->Pdf(wo, wi);
	//if (matchingComps > 1) *pdf /= matchingComps;

	if (!(bxdf->type & BSDF_SPECULAR)) 
	{
		bool reflect = (*wiW).Dot(n) * woW.Dot(n) > 0;
		for (size_t i = 0; i < m_bxdfs.size(); ++i)
			if (m_bxdfs[i]->MatchesFlags(type) &&
				((reflect && (m_bxdfs[i]->type & BSDF_REFLECTION)) ||
				(!reflect && (m_bxdfs[i]->type & BSDF_TRANSMISSION))))
				f += m_bxdfs[i]->f(wo, wi);
	}

	return f;
}

HFloat3 BSDF::WorldToReflectionCoord(const HFloat3 & v)
{
	return HFloat3(
		v.x * s.x + v.y * s.y + v.z * s.z,
		v.x * t.x + v.y * t.y + v.z * t.z,
		v.x * n.x + v.y * n.y + v.z * n.z
	);
}

HFloat3 BSDF::ReflectionToWorldCoord(const HFloat3 & v)
{
	return HFloat3(
		v.x * s.x + v.y * t.x + v.z * n.x,
		v.x * s.y + v.y * t.y + v.z * n.y,
		v.x * s.z + v.y * t.z + v.z * n.z
	);
}

HFloat3 BxDF::Sample_f(const HFloat3 & wo, HFloat3 * wi, const HFloat2 & u, HFloat * pdf) const
{
	*wi = CosineSampleHemisphere(u);
	if (wo.z < 0) wi->z *= -1;
	//*pdf = Pdf(wo, *wi);
	return f(wo, *wi);
}

HFloat3 FresnelConductor::Evaluate(HFloat cosThetaI) const
{
	return FrConductor(cosThetaI, etaI, etaT, k);
}

HFloat3 FresnelDielectric::Evaluate(HFloat cosThetaI) const
{
	HFloat val = FrDielectric(cosThetaI, etaI, etaT);
	return HFloat3(val, val, val);
}

HFloat3 LambertianReflection::f(const HFloat3 & wo, const HFloat3 & wi) const
{
	return HFloat3(R.x * H_1DIVPI, R.y * H_1DIVPI, R.z * H_1DIVPI);
}

HFloat3 CosineSampleHemisphere(const HFloat2 & u)
{
	HFloat2 d;
	HFloat2 uOffset(2.0f * u.x - 1.0f, 2.0f * u.y - 1.0f);
	if (uOffset.x == 0 && uOffset.y == 0)
	{
		d = HFloat2(0.0f, 0.0f);
	}
	else
	{
		HFloat theta, r;
		if (abs(uOffset.x) > abs(uOffset.y)) 
		{
			r = uOffset.x;
			theta = H_PIDIV4 * (uOffset.y / uOffset.x);
		}
		else 
		{
			r = uOffset.y;
			theta = H_PIDIV2 - H_PIDIV4 * (uOffset.x / uOffset.y);
		}
		d = HFloat2(r * cos(theta), r * sin(theta));
	}

	HFloat z = sqrt(max(0.0f, 1.0f - d.x * d.x - d.y * d.y));
	return HFloat3(d.x, d.y, z);
}

HFloat3 SpecularReflection::f(const HFloat3 & wo, const HFloat3 & wi) const
{
	return HFloat3(0.0f, 0.0f, 0.0f);
}

HFloat3 SpecularReflection::Sample_f(const HFloat3 & wo, HFloat3 * wi, const HFloat2 & sample, HFloat * pdf) const
{
	*wi = HFloat3(-wo.x, -wo.y, wo.z);
	*pdf = 1;

	HFloat3 fres = fresnel->Evaluate(CosTheta(*wi));
	return fres * R / AbsCosTheta(*wi);
}

HFloat3 FresnelNoOp::Evaluate(HFloat value) const
{
	return HFloat3(1.0f, 1.0f, 1.0f);
}

SpecularTransmission::SpecularTransmission(const HFloat3 & T, HFloat etaA, HFloat etaB)
	: BxDF(BxDFType(BSDF_TRANSMISSION | BSDF_SPECULAR)),
	T(T),
	etaA(etaA),
	etaB(etaB),
	fresnel(etaA, etaB) 
{
}

HFloat3 SpecularTransmission::f(const HFloat3 & wo, const HFloat3 & wi) const
{
	return HFloat3(0.0f, 0.0f, 0.0f);
}

HFloat3 SpecularTransmission::Sample_f(const HFloat3 & wo, HFloat3 * wi, const HFloat2 & sample, HFloat * pdf) const
{
	bool entering = CosTheta(wo) > 0;
	HFloat etaI = entering ? etaA : etaB;
	HFloat etaT = entering ? etaB : etaA;

	if (!Refract(wo, Faceforward(HFloat3(0, 0, 1), wo), etaI / etaT, wi))
		return HFloat3(0.0f, 0.0f, 0.0f);
	*pdf = 1;

	HFloat3 ft = T * (1.0f - fresnel.Evaluate(CosTheta(*wi)));
	if (true) ft *= (etaI * etaI) / (etaT * etaT);

	return ft / AbsCosTheta(*wi);
}

#pragma once
#include "HEngineMath.h"

enum BxDFType {
	BSDF_NONE = 0,
	BSDF_REFLECTION = 1 << 0,
	BSDF_TRANSMISSION = 1 << 1,
	BSDF_DIFFUSE = 1 << 2,
	BSDF_GLOSSY = 1 << 3,
	BSDF_SPECULAR = 1 << 4,
	BSDF_ALL = BSDF_DIFFUSE | BSDF_GLOSSY | BSDF_SPECULAR | BSDF_REFLECTION | BSDF_TRANSMISSION,
};

namespace Reflection
{
	HFloat FrDielectric(HFloat cosThetaI, HFloat etaI, HFloat etaT);
	HFloat3 FrConductor(HFloat cosThetaI, const HFloat3 &etaI, const HFloat3 &etaT, const HFloat3 &k);

	HFloat CosTheta(const HFloat3 &w);
	HFloat AbsCosTheta(const HFloat3 &w);
}

class BSDF
{
public:
	BSDF() : n(0.0f), s(0.0f), t(0.0f), eta(0.0f) {}
	BSDF(const SurfaceInteraction &si, HFloat eta = 1);
	virtual ~BSDF() {}

	void Add(BxDF* bxdf);
	HInt NumComponents(BxDFType type);

	HFloat3 f(const HFloat3 & woW, const HFloat3 & wiW, BxDFType flags = BSDF_ALL);
	HFloat3 Sample_f(const HFloat3& woW, HFloat3 *wiW, const HFloat2 &u, HFloat *pdf, BxDFType type);

	HFloat3 WorldToReflectionCoord(const HFloat3 &v);
	HFloat3 ReflectionToWorldCoord(const HFloat3 &v);

private:
	HFloat3 n, s, t;
	HFloat eta;

	static const HInt MAXBxDFs = 8;
	vector<BxDF*> m_bxdfs;
};

class BxDF
{
public:
	BxDF() = default;
	BxDF(BxDFType type) : type(type) {}
	virtual ~BxDF() {}

	bool MatchesFlags(BxDFType t) const { return (type & t) == type; }

	virtual HFloat3 f(const HFloat3 &wo, const HFloat3 &wi) const = 0;
	virtual HFloat3 Sample_f(const HFloat3 &wo, HFloat3 *wi, const HFloat2& sample, HFloat* pdf/*, BxDFType *sampledType = nullptr*/) const;

	const BxDFType type;
};

class Fresnel
{
public:
	Fresnel() {}
	virtual ~Fresnel() {}
	virtual HFloat3 Evaluate(HFloat cosI) const = 0;

private:

};

class FresnelConductor : public Fresnel
{
public:
	FresnelConductor() = default;
	FresnelConductor(const HFloat3& etaI, const HFloat3& etaT, const HFloat3& k) : etaI(etaI), etaT(etaT), k(k) {};
	HFloat3 Evaluate(HFloat cosThetaI) const;

private:
	HFloat3 etaI, etaT, k;
};

class FresnelDielectric : public Fresnel
{
public:
	FresnelDielectric() = default;
	FresnelDielectric(HFloat etaI, HFloat etaT) : etaI(etaI), etaT(etaT) {};
	HFloat3 Evaluate(HFloat cosThetaI) const;

private:
	HFloat etaI, etaT;
};

class FresnelNoOp : public Fresnel
{
public:
	FresnelNoOp() = default;
	HFloat3 Evaluate(HFloat value) const;
};

class SpecularReflection : public BxDF
{
public:
	SpecularReflection() = default;
	SpecularReflection(const HFloat3 &R, Fresnel* fresnel) : BxDF(BxDFType(BSDF_REFLECTION | BSDF_SPECULAR)), R(R), fresnel(fresnel) {}
	HFloat3 f(const HFloat3 &wo, const HFloat3 &wi) const;
	HFloat3 Sample_f(const HFloat3 &wo, HFloat3 *wi, const HFloat2 &sample,
		HFloat *pdf) const;
	//HFloat Pdf(const Vector3f &wo, const Vector3f &wi) const { return 0; }

private:
	const HFloat3 R;
	const Fresnel * fresnel;
};

class SpecularTransmission : public BxDF {
public:
	// SpecularTransmission Public Methods
	SpecularTransmission() = default;
	SpecularTransmission(const HFloat3 &T, HFloat etaA, HFloat etaB);
	HFloat3 f(const HFloat3 &wo, const HFloat3 &wi) const;
	HFloat3 Sample_f(const HFloat3 &wo, HFloat3 *wi, const HFloat2 &sample, HFloat *pdf) const;
	//HFloat Pdf(const HFloat3 &wo, const HFloat3 &wi) const { return 0; }

private:
	const HFloat3 T;
	const HFloat etaA, etaB;
	const FresnelDielectric fresnel;
};

class LambertianReflection : public BxDF
{
public:
	LambertianReflection() = default;
	LambertianReflection(const HFloat3 &R) : BxDF(BxDFType(BSDF_REFLECTION | BSDF_DIFFUSE)), R(R) {}
	HFloat3 f(const HFloat3 &wo, const HFloat3 &wi) const;

private:
	const HFloat3 R;
};

HFloat3 CosineSampleHemisphere(const HFloat2 &u);
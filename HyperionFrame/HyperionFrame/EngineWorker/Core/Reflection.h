#pragma once
#include "HMath.h"

enum BxDFType {
	BSDF_REFLECTION = 1 << 0,
	BSDF_TRANSMISSION = 1 << 1,
	BSDF_DIFFUSE = 1 << 2,
	BSDF_GLOSSY = 1 << 3,
	BSDF_SPECULAR = 1 << 4,
	BSDF_ALL = BSDF_DIFFUSE | BSDF_GLOSSY | BSDF_SPECULAR | BSDF_REFLECTION | BSDF_TRANSMISSION,
};

namespace Reflection
{
	float FrDielectric(float cosThetaI, float etaI, float etaT);
	XMCOLOR3 FrConductor(float cosThetaI, const XMCOLOR3 &etaI, const XMCOLOR3 &etaT, const XMCOLOR3 &k);

	float CosTheta(const XMFLOAT3 &w);
	float AbsCosTheta(const XMFLOAT3 &w);
}

class BSDF
{
public:
	BSDF() {}
	BSDF(const SurfaceInteraction &si, float eta = 1);
	virtual ~BSDF() {}

	void Add(BxDF* bxdf);
	int NumComponents(BxDFType type);

	XMCOLOR3 f(const XMFLOAT3 & woW, const XMFLOAT3 & wiW, BxDFType flags = BSDF_ALL);
	XMCOLOR3 Sample_f(const XMFLOAT3& woW, XMFLOAT3 *wiW, const XMFLOAT2 &u, float *pdf, BxDFType type);

	XMCOLOR3 WorldToReflectionCoord(const XMFLOAT3 &v);
	XMCOLOR3 ReflectionToWorldCoord(const XMFLOAT3 &v);

private:
	XMFLOAT3 n, s, t;
	float eta;

	static const int MAXBxDFs = 8;
	vector<BxDF*> m_bxdfs;
};

class BxDF
{
public:
	BxDF(BxDFType type) : type(type) {}
	virtual ~BxDF() {}

	bool MatchesFlags(BxDFType t) const { return (type & t) == type; }

	virtual XMCOLOR3 f(const XMFLOAT3 &wo, const XMFLOAT3 &wi) const = 0;
	virtual XMCOLOR3 Sample_f(const XMFLOAT3 &wo, XMFLOAT3 *wi, const XMFLOAT2& sample, float* pdf/*, BxDFType *sampledType = nullptr*/) const;

	const BxDFType type;
};

class Fresnel
{
public:
	Fresnel() {}
	virtual ~Fresnel() {}
	virtual XMCOLOR3 Evaluate(float cosI) const = 0;

private:

};

class FresnelConductor : public Fresnel
{
public:
	FresnelConductor(const XMCOLOR3& etaI, const XMCOLOR3& etaT, const XMCOLOR3& k) : etaI(etaI), etaT(etaT), k(k) {};
	XMCOLOR3 Evaluate(float cosThetaI) const;

private:
	XMCOLOR3 etaI, etaT, k;
};

class FresnelDielectric : public Fresnel
{
public:
	FresnelDielectric(float etaI, float etaT) : etaI(etaI), etaT(etaT) {};
	XMCOLOR3 Evaluate(float cosThetaI) const;

private:
	float etaI, etaT;
};

class FresnelNoOp : public Fresnel
{
public:
	XMCOLOR3 Evaluate(float value) const;
};

class SpecularReflection : public BxDF
{
public:
	SpecularReflection(const XMCOLOR3 &R, Fresnel* fresnel) : BxDF(BxDFType(BSDF_REFLECTION | BSDF_SPECULAR)), R(R), fresnel(fresnel) {}
	XMCOLOR3 f(const XMFLOAT3 &wo, const XMFLOAT3 &wi) const;
	XMCOLOR3 Sample_f(const XMFLOAT3 &wo, XMFLOAT3 *wi, const XMFLOAT2 &sample,
		float *pdf) const;
	//float Pdf(const Vector3f &wo, const Vector3f &wi) const { return 0; }

private:
	const XMCOLOR3 R;
	const Fresnel * fresnel;
};

class SpecularTransmission : public BxDF {
public:
	// SpecularTransmission Public Methods
	SpecularTransmission(const XMCOLOR3 &T, float etaA, float etaB);
	XMCOLOR3 f(const XMFLOAT3 &wo, const XMFLOAT3 &wi) const;
	XMCOLOR3 Sample_f(const XMFLOAT3 &wo, XMFLOAT3 *wi, const XMFLOAT2 &sample,
		float *pdf) const;
	//float Pdf(const XMFLOAT3 &wo, const XMFLOAT3 &wi) const { return 0; }

private:
	const XMCOLOR3 T;
	const float etaA, etaB;
	const FresnelDielectric fresnel;
};

class LambertianReflection : public BxDF
{
public:
	LambertianReflection(const XMCOLOR3 &R) : BxDF(BxDFType(BSDF_REFLECTION | BSDF_DIFFUSE)), R(R) {}
	XMCOLOR3 f(const XMFLOAT3 &wo, const XMFLOAT3 &wi) const;

private:
	const XMCOLOR3 R;
};

XMFLOAT3 CosineSampleHemisphere(const XMFLOAT2 &u);
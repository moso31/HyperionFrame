#pragma once
#include "Interaction.h"

enum BxDFType {
	BSDF_REFLECTION = 1 << 0,
	BSDF_TRANSMISSION = 1 << 1,
	BSDF_DIFFUSE = 1 << 2,
	BSDF_GLOSSY = 1 << 3,
	BSDF_SPECULAR = 1 << 4,
	BSDF_ALL = BSDF_DIFFUSE | BSDF_GLOSSY | BSDF_SPECULAR | BSDF_REFLECTION | BSDF_TRANSMISSION,
};

class BSDF
{
public:
	BSDF() {}
	BSDF(const SurfaceInteraction &si, float eta = 1);
	~BSDF() {}

	XMCOLOR3 f(const XMFLOAT3 & woW, const XMFLOAT3 & wiW, BxDFType flags = BSDF_ALL);

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
	~BxDF() {}

	bool MatchesFlags(BxDFType t) const { return (type & t) == type; }

	virtual XMCOLOR3 f(const XMFLOAT3 &wo, const XMFLOAT3 &wi) const = 0;
	//virtual XMCOLOR3 Sample_f(const XMFLOAT3 &wo, XMFLOAT3 *wi, BxDFType type = BSDF_ALL, BxDFType *sampledType = nullptr) const;

	const BxDFType type;
};

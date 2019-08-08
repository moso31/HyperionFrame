#pragma once
#include "Interaction.h"
#include "Reflection.h"

enum eMaterialType
{
	HMAT_DEFAULT,
	HMAT_MATTE,
	HMAT_GLASS,
	HMAT_MIRROR,
	HMAT_PLASTIC,
};

class HPBRMaterial
{
public:
	HPBRMaterial(eMaterialType mtrlType = HMAT_DEFAULT);
	virtual ~HPBRMaterial();

	virtual void ComputeScatterFunction(SurfaceInteraction* si) = 0;

	eMaterialType GetType() { return m_type; }
	void SetType(eMaterialType type) { m_type = type; }

	// 获取该材质的漫反射颜色
	virtual HFloat3 GetDiffuse() { return HFloat3(0.5f, 0.5f, 0.5f); }
	virtual void SetDiffuse(HFloat3 diffuse) {};

private:
	eMaterialType m_type;
};

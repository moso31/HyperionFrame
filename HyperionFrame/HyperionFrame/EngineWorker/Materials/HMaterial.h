#pragma once
#include "Interaction.h"
#include "Reflection.h"

enum eMaterialType
{
	HMAT_DEFAULT,
	HMAT_MATTE,
	HMAT_GLASS,
	HMAT_MIRROR,
};

class HMaterial
{
public:
	HMaterial(eMaterialType mtrlType = HMAT_DEFAULT);
	virtual ~HMaterial();

	virtual void ComputeScatterFunction(SurfaceInteraction* si) = 0;

	eMaterialType GetType() { return m_type; }
	void SetType(eMaterialType type) { m_type = type; }

	// 获取该材质的漫反射颜色
	// 默认显示50%灰色，用于镜面等不具备漫反射的物体。
	virtual HFloat3 GetDiffuse() { return HFloat3(0.5f, 0.5f, 0.5f); }
	virtual void SetDiffuse(HFloat3 diffuse) {};

private:
	eMaterialType m_type;
};

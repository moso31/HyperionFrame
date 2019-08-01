#pragma once
#include "HPBRMaterial.h"

class HPBRMaterialMatte : public HPBRMaterial
{
public:
	HPBRMaterialMatte() = default;
	HPBRMaterialMatte(const HFloat3& _Kd, const HFloat& _sigma);
	~HPBRMaterialMatte();

	void ComputeScatterFunction(SurfaceInteraction* si);

	HFloat3 GetDiffuse() { return Kd; }
	void SetDiffuse(HFloat3 diffuse) { Kd = diffuse; }

private:
	HFloat3 Kd;
	HFloat sigma;
};

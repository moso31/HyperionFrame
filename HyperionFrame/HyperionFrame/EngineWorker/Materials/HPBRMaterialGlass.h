#pragma once
#include "HPBRMaterial.h"

class HPBRMaterialGlass : public HPBRMaterial
{
public:
	HPBRMaterialGlass() = default;
	HPBRMaterialGlass(const HFloat3 & _Kr, const HFloat3 & _Kt, HFloat _eta);
	~HPBRMaterialGlass();

	void ComputeScatterFunction(SurfaceInteraction* si);

private:
	HFloat3 Kr, Kt;
	HFloat eta;
};

#pragma once
#include "HPBRMaterial.h"

class HPBRMaterialMirror : public HPBRMaterial
{
public:
	HPBRMaterialMirror() = default;
	HPBRMaterialMirror(const HFloat3& _Kr);
	~HPBRMaterialMirror();

	void ComputeScatterFunction(SurfaceInteraction* si);

private:
	HFloat3 Kr;
};

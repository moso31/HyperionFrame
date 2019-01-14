#pragma once
#include "HShape.h"

class Box : public HShape
{
public:
	Box();
	Box(const shared_ptr<DXResource>& dxResources);
	~Box();

	ePrimitiveType GetType() { return BOX; }

	void InitParameters();

	void Update(UINT8 * destination);
	void Render(ComPtr<ID3D12GraphicsCommandList> pCommandList);

	bool Intersect(Ray worldRay, SurfaceInteraction* out_isect, float* out_tHit);
	bool IntersectP(Ray worldRay, float* out_t0, float* out_t1);
};

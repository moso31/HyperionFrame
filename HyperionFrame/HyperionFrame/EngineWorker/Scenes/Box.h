#pragma once
#include "Shape.h"

class Box : public Shape
{
public:
	Box();
	Box(const shared_ptr<DXResource>& dxResources);
	~Box();

	eShapeType GetType() { return HSHAPE_BOX; }

	void InitParameters();

	void Update(UINT8 * destination);
	void Render(ComPtr<ID3D12GraphicsCommandList> pCommandList);

	bool Intersect(Ray worldRay, SurfaceInteraction* out_isect, float* out_tHit);
	bool IntersectP(Ray worldRay, float* out_t0, float* out_t1);
};

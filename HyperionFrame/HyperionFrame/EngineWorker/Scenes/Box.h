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

	void Update();
	void Render(ComPtr<ID3D12GraphicsCommandList> pCommandList);

	bool Intersect(Ray worldRay, SurfaceInteraction* out_isect, EFloat* out_tHit);
	bool IntersectP(Ray worldRay, EFloat* out_t0, EFloat* out_t1);
};

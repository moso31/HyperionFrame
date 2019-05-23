#pragma once
#include "HShape.h"

class Sphere : public HShape
{
public:
	Sphere();
	Sphere(const shared_ptr<DXResource>& dxResources);
	~Sphere();

	ePrimitiveType GetType() { return m_type; }

	void InitParameters(HFloat radius = 1.0f, HInt segmentHorizontal = 16, HInt segmentVertical = 16);

	void Update();
	void Render(ComPtr<ID3D12GraphicsCommandList> pCommandList);

	bool Intersect(Ray worldRay, SurfaceInteraction* out_isect, EFloat* out_tHit);
	bool IntersectP(Ray worldRay, EFloat* out_t0, EFloat* out_t1);

private:
	HFloat m_radius;
	HInt m_segmentVertical;
	HInt m_segmentHorizontal;
};

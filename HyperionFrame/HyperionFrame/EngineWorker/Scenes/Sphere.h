#pragma once
#include "HShape.h"

class Sphere : public HShape
{
public:
	Sphere();
	Sphere(const shared_ptr<DXResource>& dxResources);
	~Sphere();

	ePrimitiveType GetType() { return m_type; }

	void InitParameters(float radius = 1.0f, int segmentHorizontal = 16, int segmentVertical = 16);

	void Update();
	void Render(ComPtr<ID3D12GraphicsCommandList> pCommandList);

	bool Intersect(Ray worldRay, SurfaceInteraction* out_isect, EFloat* out_tHit);
	bool IntersectP(Ray worldRay, EFloat* out_t0, EFloat* out_t1);

private:
	float m_radius;
	int m_segmentVertical;
	int m_segmentHorizontal;
};

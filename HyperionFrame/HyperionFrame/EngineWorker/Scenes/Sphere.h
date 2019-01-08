#pragma once
#include "Shape.h"

class Sphere : public Shape
{
public:
	Sphere();
	Sphere(const shared_ptr<DXResource>& dxResources);
	~Sphere();

	eShapeType GetType() { return m_type; }

	void InitParameters(float radius = 1.0f, int segmentHorizontal = 16, int segmentVertical = 16);

	void Update(UINT8 * destination);
	void Render(ComPtr<ID3D12GraphicsCommandList> pCommandList);

	bool Intersect(Ray worldRay, SurfaceInteraction* out_isect, float* out_tHit);
	bool IntersectP(Ray worldRay, float* out_t0, float* out_t1);

private:
	float m_radius;
	int m_segmentVertical;
	int m_segmentHorizontal;
};

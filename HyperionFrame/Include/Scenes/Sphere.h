#pragma once
#include "Shape.h"

class Sphere : public Shape
{
public:
	Sphere();
	Sphere(const shared_ptr<DXResource>& dxResources);
	~Sphere();

	void Init(ComPtr<ID3D12GraphicsCommandList> pCommandList, float radius = 1.0f, int segmentVertical = 16, int segmentHorizontal = 16);

	void Update(UINT8 * destination);
	void Render(ComPtr<ID3D12GraphicsCommandList> pCommandList);

	bool Intersect(Ray worldRay, SurfaceInteraction* out_isect, float* out_tHit);
	bool IntersectP(Ray worldRay, float* out_t0, float* out_t1);

private:
	void _initBufferData(ComPtr<ID3D12GraphicsCommandList> pCommandList);
	void _initParameters(float radius = 1.0f, int segmentVertical = 16, int segmentHorizontal = 16);

	float m_radius;
	int m_segmentVertical;
	int m_segmentHorizontal;
};

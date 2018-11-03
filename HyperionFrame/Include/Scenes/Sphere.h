#pragma once
#include "Shape.h"

class Sphere : public Shape
{
public:
	static const UINT c_alignedConstantBufferSize = (sizeof(ModelViewProjectionConstantBuffer) + 255) & ~255; // 常量缓冲区大小必须都是 256 字节的整数倍。

	Sphere();
	Sphere(const shared_ptr<DXResource>& dxResources, Camera* camera);
	~Sphere();

	void Init(ComPtr<ID3D12GraphicsCommandList> pCommandList, float radius = 1.0f, int segmentVertical = 16, int segmentHorizontal = 16);

	void Update(UINT8 * destination);
	void Render(ComPtr<ID3D12GraphicsCommandList> pCommandList);

	void Intersect(Ray worldRay, SurfaceInteraction* out_isect);
	bool IntersectP(Ray worldRay);

private:
	void _initBufferData(ComPtr<ID3D12GraphicsCommandList> pCommandList);
	void _initParameters(float radius = 1.0f, int segmentVertical = 16, int segmentHorizontal = 16);

	float m_radius;
	int m_segmentVertical;
	int m_segmentHorizontal;
};

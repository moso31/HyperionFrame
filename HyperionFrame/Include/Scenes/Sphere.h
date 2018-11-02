#pragma once
#include "Shape.h"

class Sphere : public Shape
{
public:
	static const UINT c_alignedConstantBufferSize = (sizeof(ModelViewProjectionConstantBuffer) + 255) & ~255; // ������������С���붼�� 256 �ֽڵ���������

	Sphere();
	Sphere(const shared_ptr<DXResource>& dxResources, Camera* camera);
	~Sphere();

	void Init(ComPtr<ID3D12GraphicsCommandList> pCommandList);

	void Update(UINT8 * destination);
	void Render(ComPtr<ID3D12GraphicsCommandList> pCommandList);

	void Intersect(Ray worldRay, int& out_hitIndex, SurfaceInteraction* out_isect);
	bool IntersectP(Ray worldRay);

private:
	void _initBufferData(ComPtr<ID3D12GraphicsCommandList> pCommandList);
	void _initParameters();
};

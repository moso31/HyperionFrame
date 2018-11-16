#pragma once
#include "Shape.h"

class Box : public Shape
{
public:
	static const UINT c_alignedConstantBufferSize = (sizeof(ModelViewProjectionConstantBuffer) + 255) & ~255; // 常量缓冲区大小必须都是 256 字节的整数倍。

	Box();
	Box(const shared_ptr<DXResource>& dxResources);
	~Box();

	void Init(ComPtr<ID3D12GraphicsCommandList> pCommandList, float length = 1.0f, float width = 1.0f, float height = 1.0f);

	void Update(UINT8 * destination);
	void Render(ComPtr<ID3D12GraphicsCommandList> pCommandList);

	void Intersect(Ray worldRay, SurfaceInteraction* out_isect);
	bool IntersectP(Ray worldRay);

private:
	void _initBufferData(ComPtr<ID3D12GraphicsCommandList> pCommandList);
	void _initParameters(float length = 1.0f, float width = 1.0f, float height = 1.0f);
};

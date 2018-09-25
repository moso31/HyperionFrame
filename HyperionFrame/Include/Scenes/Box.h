#pragma once
#include "Shape.h"

class Box : public Shape
{
public:
	static const UINT c_alignedConstantBufferSize = (sizeof(ModelViewProjectionConstantBuffer) + 255) & ~255; // ������������С���붼�� 256 �ֽڵ���������

	Box();
	Box(const shared_ptr<DXResource>& dxResources, Camera* camera);
	~Box();

	void Init(ComPtr<ID3D12GraphicsCommandList> pCommandList);

	void Update(UINT8 * destination);
	void Render(ComPtr<ID3D12GraphicsCommandList> pCommandList);

	void ReleaseUploadBuffers();

	void Intersect(Ray worldRay, XMFLOAT3& out_hitPos, int& out_hitIndex, SurfaceInteraction* out_isect);
	bool IntersectP(Ray worldRay);

private:
	void _initBufferData(ComPtr<ID3D12GraphicsCommandList> pCommandList);
	void _initParameters();
};

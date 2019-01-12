#pragma once
#include "HPrimitive.h"

struct Triangle
{
	union
	{
		struct
		{
			XMFLOAT3 A, B, C;
		};
		XMFLOAT3 p[3];
	};
};

struct TriangleUV
{
	union
	{
		struct
		{
			XMFLOAT2 A, B, C;
		};
		XMFLOAT2 p[3];
	};
};

class Shape : public HPrimitive
{
public:
	Shape();
	Shape(const shared_ptr<DXResource>& dxResources);
	virtual ~Shape();

	virtual void Update(UINT8 * destination) = 0;
	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pCommandList) = 0;

	UINT GetFaceCount();
	Triangle GetFace(UINT faceIndex);
	TriangleUV GetUVs(UINT faceIndex);

	shared_ptr<HMaterial> GetMaterial() { return m_material; }
	void SetMaterial(shared_ptr<HMaterial> mat);

	virtual bool Intersect(Ray worldRay, SurfaceInteraction* out_isect, float* out_tHit) = 0;
	virtual bool IntersectP(Ray worldRay, float* out_t0, float* out_t1) = 0;

protected:
	shared_ptr<DXResource>		m_dxResources;

	shared_ptr<HMaterial>		m_material;
	CBufferMeshData				m_cbMeshData;
};

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

class HShape : public HPrimitive
{
public:
	HShape();
	HShape(const shared_ptr<DXResource>& dxResources);
	virtual ~HShape();

	virtual void Update() = 0;
	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pCommandList) = 0;

	UINT GetFaceCount();
	Triangle GetFace(UINT faceIndex);
	TriangleUV GetUVs(UINT faceIndex);

	shared_ptr<HMaterial> GetMaterial() { return m_material; }
	void SetMaterial(shared_ptr<HMaterial> mat);

	virtual bool Intersect(Ray worldRay, SurfaceInteraction* out_isect, EFloat* out_tHit) = 0;
	virtual bool IntersectP(Ray worldRay, EFloat* out_t0, EFloat* out_t1) = 0;

	virtual UINT GetAlignedConstantBufferSize();

protected:
	shared_ptr<DXResource>		m_dxResources;

	shared_ptr<HMaterial>		m_material;
	CBufferMeshData				m_cbMeshData;
};

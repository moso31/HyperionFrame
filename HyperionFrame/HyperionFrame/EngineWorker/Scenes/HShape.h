#pragma once
#include "HPrimitive.h"

struct Triangle
{
	HFloat3 p[3];
};

struct TriangleUV
{
	HFloat2 p[3];
};

class HShape : public HPrimitive
{
public:
	HShape();
	HShape(const shared_ptr<DXResource>& dxResources);
	virtual ~HShape();

	virtual void Update() = 0;
	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pCommandList) = 0;

	HUInt GetFaceCount();
	Triangle GetFace(HUInt faceIndex);
	TriangleUV GetUVs(HUInt faceIndex);

	shared_ptr<HPBRMaterial> GetPBRMaterial() { return m_materialPBR; }
	void SetPBRMaterial(shared_ptr<HPBRMaterial> mat);

	virtual bool Intersect(Ray worldRay, SurfaceInteraction* out_isect, EFloat* out_tHit) = 0;
	virtual bool IntersectP(Ray worldRay, EFloat* out_t0, EFloat* out_t1) = 0;

	virtual HUInt GetAlignedConstantBufferSize();

protected:
	shared_ptr<DXResource>		m_dxResources;

	shared_ptr<HPBRMaterial>	m_materialPBR;
	CBufferMeshData				m_cbMeshData;
};

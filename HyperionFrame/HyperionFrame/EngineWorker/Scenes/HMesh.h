#pragma once
#include "HShape.h"

class HMesh : public HShape, public enable_shared_from_this<HMesh>
{
public:
	HMesh();
	HMesh(const shared_ptr<DXResource>& dxResources);
	~HMesh();

	ePrimitiveType GetType() { return BOX; }

	void AddVertex(VertexPNT vertex) { m_vertices.push_back(vertex); }
	void AddIndex(USHORT index) { m_indices.push_back(index); }
	void InitParameters(string filepath);

	void Update();
	void Render(ComPtr<ID3D12GraphicsCommandList> pCommandList);

	bool Intersect(Ray worldRay, SurfaceInteraction* out_isect, EFloat* out_tHit);
	bool IntersectP(Ray worldRay, EFloat* out_t0, EFloat* out_t1);

private:
	string m_filepath;
};
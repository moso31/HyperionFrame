#include "HShape.h"
#include "DirectXHelper.h"

#include "HMatteMaterial.h"
#include "HGlassMaterial.h"
#include "HMirrorMaterial.h"

HShape::HShape()
{
}

HShape::HShape(const shared_ptr<DXResource>& dxResources) :
	HPrimitive(dxResources)
{
}

HShape::~HShape()
{
}

UINT HShape::GetFaceCount()
{
	return (UINT)m_indices.size() / 3;
}

Triangle HShape::GetFace(UINT faceIndex)
{
	int index = faceIndex * 3;
	Triangle tri;
	tri.p[0] = m_vertices[m_indices[index]].pos;
	tri.p[1] = m_vertices[m_indices[index + 1]].pos;
	tri.p[2] = m_vertices[m_indices[index + 2]].pos;
	return tri;
}

TriangleUV HShape::GetUVs(UINT faceIndex)
{
	int index = faceIndex * 3;
	TriangleUV triUV;
	triUV.p[0] = m_vertices[m_indices[index]].uv;
	triUV.p[1] = m_vertices[m_indices[index + 1]].uv;
	triUV.p[2] = m_vertices[m_indices[index + 2]].uv;
	return triUV;
}

void HShape::SetMaterial(shared_ptr<HMaterial> mat)
{
	m_material = mat;
	m_cbMeshData.vertColor = mat->GetDiffuse();
}

UINT HShape::GetAlignedConstantBufferSize()
{
	return ((sizeof(ModelViewProjectionConstantBuffer) + 255) + (sizeof(CBufferMeshData) + 255)) & ~255;
}

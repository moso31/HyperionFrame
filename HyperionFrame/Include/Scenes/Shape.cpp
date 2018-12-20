#include "Shape.h"
#include "HMatteMaterial.h"
#include "HGlassMaterial.h"
#include "HMirrorMaterial.h"

Shape::Shape()
{
}

Shape::Shape(const shared_ptr<DXResource>& dxResources) :
	m_dxResources(dxResources)
{
}

Shape::~Shape()
{
}

AABB Shape::GetAABB()
{
	return m_aabb;
}

AABB Shape::GetAABBWorld()
{
	XMVECTOR maxV = XMLoadFloat3(&m_aabb.GetVecMax());
	XMVECTOR minV = XMLoadFloat3(&m_aabb.GetVecMin());
	XMMATRIX mxObject2World = XMLoadFloat4x4(&worldMatrix);
	XMFLOAT3 max, min;
	XMStoreFloat3(&max, XMVector3TransformCoord(maxV, mxObject2World));
	XMStoreFloat3(&min, XMVector3TransformCoord(minV, mxObject2World));
	return AABB(min, max);
}

UINT Shape::GetFaceCount()
{
	return (UINT)m_indices.size() / 3;
}

Triangle Shape::GetFace(UINT faceIndex)
{
	int index = faceIndex * 3;
	Triangle tri;
	tri.p[0] = m_vertices[m_indices[index]].pos;
	tri.p[1] = m_vertices[m_indices[index + 1]].pos;
	tri.p[2] = m_vertices[m_indices[index + 2]].pos;
	return tri;
}

TriangleUV Shape::GetUVs(UINT faceIndex)
{
	int index = faceIndex * 3;
	TriangleUV triUV;
	triUV.p[0] = m_vertices[m_indices[index]].uv;
	triUV.p[1] = m_vertices[m_indices[index + 1]].uv;
	triUV.p[2] = m_vertices[m_indices[index + 2]].uv;
	return triUV;
}

void Shape::SetMaterial(HMaterial * mat)
{
	m_material = mat;
	m_cbDataMaterial.vertColor = mat->GetDiffuse();
}

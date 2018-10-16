#include "Shape.h"

Shape::Shape()
{
}

Shape::Shape(const shared_ptr<DXResource>& dxResources, Camera * camera) :
	m_dxResources(dxResources),
	m_camera(camera)
{
}

Shape::~Shape()
{
}

AABB Shape::GetAABB()
{
	return m_aabb;
}

AABB Shape::GenerateAABB()
{
	AABB result;
	for (size_t i = 0; i < m_vertices.size(); i++)
		result.Merge(m_vertices[i].pos);
	return result;
}

UINT Shape::GetFaceCount()
{
	return (UINT)m_indices.size() / 3;
}

void Shape::GetFace(UINT faceIndex, XMFLOAT3 out_face[3])
{
	int index = faceIndex * 3;
	out_face[0] = m_vertices[m_indices[index]].pos;
	out_face[1] = m_vertices[m_indices[index + 1]].pos;
	out_face[2] = m_vertices[m_indices[index + 2]].pos;
}

void Shape::GetUVs(UINT faceIndex, XMFLOAT2 out_uv[3])
{
	int index = faceIndex * 3;
	out_uv[0] = m_vertices[m_indices[index]].uv;
	out_uv[1] = m_vertices[m_indices[index + 1]].uv;
	out_uv[2] = m_vertices[m_indices[index + 2]].uv;
}

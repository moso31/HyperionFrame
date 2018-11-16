#include "Shape.h"

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

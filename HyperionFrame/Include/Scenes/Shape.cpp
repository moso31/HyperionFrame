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
	return m_indices.size() / 3;
}

HTriangle Shape::GetFace(UINT faceIndex)
{
	HTriangle result;
	for (int i = 0; i < 3; i++)
		result.p[i] = m_vertices[m_indices[faceIndex * 3 + i]].pos;
	return result;
}

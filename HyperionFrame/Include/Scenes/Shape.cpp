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

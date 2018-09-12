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

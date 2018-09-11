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

Transform Shape::GetTransform()
{
	return Transform(*this);
}

XMMATRIX Shape::GetTransformMatrix()
{
	return Transform(*this).GetMatrix();
}

void Shape::SetTranslation(float x, float y, float z)
{
	translation = XMFLOAT3(x, y, z);
}

void Shape::SetRotation(float x, float y, float z)
{
	rotation = XMFLOAT3(x, y, z);
}

void Shape::SetScale(float x, float y, float z)
{
	scale = XMFLOAT3(x, y, z);
}

#include "Shape.h"

Shape::Shape()
{
}

Shape::~Shape()
{
}

Transform Shape::GetTransform()
{
	return m_transform;
}

XMMATRIX Shape::GetTransformMatrix()
{
	return m_transform.GetMatrix();
}

void Shape::SetTranslation(float x, float y, float z)
{
	m_transform.translation = XMFLOAT3(x, y, z);
}

void Shape::SetRotation(float x, float y, float z)
{
	m_transform.rotation = XMFLOAT3(x, y, z);
}

void Shape::SetScale(float x, float y, float z)
{
	m_transform.scale = XMFLOAT3(x, y, z);
}

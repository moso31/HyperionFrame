#pragma once
#include "Transform.h"

class Shape
{
public:
	Shape();
	~Shape();

	Transform	GetTransform();
	XMMATRIX	GetTransformMatrix();

	void SetTranslation(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);

private:
	Transform	m_transform;
};

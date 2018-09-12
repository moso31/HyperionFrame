#include "Transform.h"

Transform::Transform(XMFLOAT3 _translation, XMFLOAT3 _rotation, XMFLOAT3 _scale) :
	translation(_translation),
	rotation(_rotation),
	scale(_scale)
{
}

XMFLOAT3 Transform::GetTranslation()
{
	return translation;
}

XMFLOAT3 Transform::GetRotation()
{
	return rotation;
}

XMFLOAT3 Transform::GetScale()
{
	return scale;
}

void Transform::SetTranslation(float x, float y, float z)
{
	translation = { x, y, z };
}

void Transform::SetRotation(float x, float y, float z)
{
	rotation = { x, y, z };
}

void Transform::SetScale(float x, float y, float z)
{
	scale = { x, y, z };
}

Transform Transform::GetTransform()
{
	return Transform(*this);
}

XMMATRIX Transform::GetTransformMatrix()
{
	XMVECTOR vTran = XMLoadFloat3(&translation);
	XMVECTOR vRota = XMLoadFloat3(&rotation);
	XMVECTOR vScal = XMLoadFloat3(&scale);

	return XMMatrixScalingFromVector(vScal) * XMMatrixRotationRollPitchYawFromVector(vRota) * XMMatrixTranslationFromVector(vTran);
}

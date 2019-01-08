#include "Transform.h"

Transform::Transform(XMFLOAT3 _translation, XMFLOAT3 _rotation, XMFLOAT3 _scale) :
	translation(_translation),
	rotation(_rotation),
	scale(_scale)
{
}

void Transform::UpdateTransformData()
{
	XMVECTOR vTran = XMLoadFloat3(&translation);
	XMVECTOR vRota = XMLoadFloat3(&rotation);
	XMVECTOR vScal = XMLoadFloat3(&scale);

	XMMATRIX mxObject2World = XMMatrixScalingFromVector(vScal) * XMMatrixRotationRollPitchYawFromVector(vRota) * XMMatrixTranslationFromVector(vTran);

	XMStoreFloat4x4(&worldMatrix, mxObject2World);
	XMStoreFloat4x4(&worldMatrixInv, XMMatrixInverse(&XMMatrixDeterminant(mxObject2World), mxObject2World));
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

XMFLOAT4X4 Transform::GetObject2World()
{
	return worldMatrix;
}

XMFLOAT4X4 Transform::GetWorld2Object()
{
	return worldMatrixInv;
}

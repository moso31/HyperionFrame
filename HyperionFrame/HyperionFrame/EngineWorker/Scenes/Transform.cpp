#include "Transform.h"

Transform::Transform(HFloat3 _translation, HFloat3 _rotation, HFloat3 _scale) :
	translation(_translation),
	rotation(_rotation),
	scale(_scale)
{
}

void Transform::UpdateTransformData()
{
	HFloat4x4 mxS, mxR, mxT;
	mxS.SetScale(scale);
	mxR.SetRotationXYZ(rotation);
	mxT.SetTranslation(translation);

	worldMatrix = mxS * mxR * mxT;
	worldMatrixInv = worldMatrix.Inverse();
}

HFloat3 Transform::GetTranslation()
{
	return translation;
}

HFloat3 Transform::GetRotation()
{
	return rotation;
}

HFloat3 Transform::GetScale()
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

HFloat4x4 Transform::GetObject2World()
{
	return worldMatrix;
}

HFloat4x4 Transform::GetWorld2Object()
{
	return worldMatrixInv;
}

#include "Transform.h"

Transform::Transform(XMFLOAT3 _translation, XMFLOAT3 _rotation, XMFLOAT3 _scale) :
	translation(_translation),
	rotation(_rotation),
	scale(_scale)
{
}

XMMATRIX Transform::GetMatrix()
{
	XMVECTOR vTran = XMLoadFloat3(&translation);
	XMVECTOR vRota = XMLoadFloat3(&rotation);
	XMVECTOR vScal = XMLoadFloat3(&scale);

	return XMMatrixScalingFromVector(vScal) * XMMatrixRotationRollPitchYawFromVector(vRota) * XMMatrixTranslationFromVector(vTran);
}

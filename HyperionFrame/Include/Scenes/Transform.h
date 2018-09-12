#pragma once
#include "HMath.h"

using namespace DirectX;

class Transform
{
public:
	Transform(XMFLOAT3 _translation = XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3 _rotation = XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3 _scale = XMFLOAT3(1.0f, 1.0f, 1.0f));

	XMFLOAT3 GetTranslation();
	XMFLOAT3 GetRotation();
	XMFLOAT3 GetScale();
	void SetTranslation(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);

	Transform	GetTransform();
	XMMATRIX	GetTransformMatrix();

protected:
	XMFLOAT3 translation;
	XMFLOAT3 rotation;
	XMFLOAT3 scale;
};
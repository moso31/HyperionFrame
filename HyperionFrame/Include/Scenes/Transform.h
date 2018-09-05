#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class Transform
{
public:
	Transform(XMFLOAT3 _translation = XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3 _rotation = XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3 _scale = XMFLOAT3(1.0f, 1.0f, 1.0f));

	XMMATRIX GetMatrix();

public:
	XMFLOAT3 translation;
	XMFLOAT3 rotation;
	XMFLOAT3 scale;
};
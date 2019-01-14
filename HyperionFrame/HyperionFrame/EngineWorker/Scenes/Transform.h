#pragma once
#include "HMath.h"
#include "HObject.h"

using namespace DirectX;

class Transform : public HObject
{
public:
	Transform(XMFLOAT3 _translation = XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3 _rotation = XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3 _scale = XMFLOAT3(1.0f, 1.0f, 1.0f));
	virtual ~Transform() {};

	void UpdateTransformData();

	XMFLOAT3 GetTranslation();
	XMFLOAT3 GetRotation();
	XMFLOAT3 GetScale();
	void SetTranslation(float x, float y, float z);
	virtual void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);

	Transform	GetTransform();
	XMFLOAT4X4	GetObject2World();
	XMFLOAT4X4	GetWorld2Object();

protected:
	XMFLOAT3 translation;
	XMFLOAT3 rotation;
	XMFLOAT3 scale;

	XMFLOAT4X4 worldMatrix, worldMatrixInv;
};
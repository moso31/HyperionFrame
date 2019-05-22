#pragma once
#include "HEngineMath.h"
#include "HObject.h"

class Transform : public HObject
{
public:
	Transform(HFloat3 _translation = HFloat3(0.0f, 0.0f, 0.0f), HFloat3 _rotation = HFloat3(0.0f, 0.0f, 0.0f), HFloat3 _scale = HFloat3(1.0f, 1.0f, 1.0f));
	virtual ~Transform() {};

	void UpdateTransformData();

	HFloat3 GetTranslation();
	HFloat3 GetRotation();
	HFloat3 GetScale();
	virtual void SetTranslation(float x, float y, float z);
	virtual void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);

	Transform	GetTransform();
	HFloat4x4	GetObject2World();
	HFloat4x4	GetWorld2Object();

	ComPtr<ID3D12Resource>	GetConstantBuffer()			{ return m_constantBuffer; }
	UINT8*					GetMappedConstantBuffer()	{ return m_mappedConstantBuffer; }

protected:
	HFloat3 translation;
	HFloat3 rotation;
	HFloat3 scale;

	HFloat4x4	worldMatrix, worldMatrixInv;

	ComPtr<ID3D12Resource>		m_constantBuffer;
	UINT8*						m_mappedConstantBuffer;
};
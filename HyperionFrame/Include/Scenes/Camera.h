#pragma once
#include "DXResource.h"
#include "DirectXHelper.h"
#include "FileReader.h"
#include "ShaderStructures.h"

#include "Transform.h"

using namespace HIFrameMath;

class Camera : public Transform
{
public:
	Camera();
	Camera(const shared_ptr<DXResource>& dxResources);
	~Camera();

	D3D12_RECT GetScissorRect() { return m_scissorRect; }

	void Init(float m_nearZ = 0.01f, float m_farZ = 100.0f);
	void OnResize();

	void Update();
	void Render();

	void SetRotation(float x, float y, float z);
	void SetLookAt(float x, float y, float z);
	
	XMFLOAT3 GetAt();
	XMFLOAT3 GetUp();

	Ray GenerateRay(float screenX, float screenY);

private:
	shared_ptr<DXResource> m_dxResources;

	D3D12_RECT											m_scissorRect;

	XMFLOAT4X4 m_projectionMatrix;
	XMFLOAT4X4 m_viewMatrix;

	float m_nearZ, m_farZ;
	XMFLOAT3 m_at, m_up;
};

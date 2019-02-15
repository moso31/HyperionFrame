#pragma once
#include "DXResource.h"
#include "ShaderStructures.h"

#include "Transform.h"

class Camera : public Transform
{
public:
	Camera();
	Camera(const shared_ptr<DXResource>& dxResources);
	~Camera();

	D3D12_RECT GetScissorRect() { return m_scissorRect; }

	void Init(float fovY = 70.0f, float nearZ = 0.01f, float farZ = 100.0f);
	void OnResize();

	void Update();
	void Render();

	void SetTranslation(float x, float y, float z) override;
	void SetRotation(float x, float y, float z) override;
	void SetLookAt(float x, float y, float z);
	
	XMFLOAT3 GetForward();
	XMFLOAT3 GetLeft();
	XMFLOAT3 GetAt();
	XMFLOAT3 GetUp();

	XMFLOAT4X4 GetViewToWorld(XMMATRIX& out_mxResult);
	Ray GenerateRay(float screenX, float screenY);

	virtual UINT GetAlignedConstantBufferSize();
	virtual void SetCameraBuffer();

private:
	shared_ptr<DXResource> m_dxResources;

	D3D12_RECT m_scissorRect;

	XMFLOAT4X4 m_projectionMatrix;
	XMFLOAT4X4 m_viewMatrix;

	float m_fovY;
	float m_nearZ, m_farZ;
	XMFLOAT3 m_at, m_up;

	CBufferEyePos		m_cbEyePos;
};

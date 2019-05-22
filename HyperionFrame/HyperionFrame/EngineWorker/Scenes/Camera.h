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

	void Init(HFloat fovY = 70.0f, HFloat nearZ = 0.01f, HFloat farZ = 100.0f);
	void OnResize();

	void Update();
	void Render();

	void SetTranslation(HFloat x, HFloat y, HFloat z) override;
	void SetRotation(HFloat x, HFloat y, HFloat z) override;
	void SetLookAt(HFloat x, HFloat y, HFloat z);
	
	HFloat3 GetForward();
	HFloat3 GetLeft();
	HFloat3 GetAt();
	HFloat3 GetUp();

	HFloat4x4 GetViewToWorld(HFloat4x4& out_mxResult);
	Ray GenerateRay(HFloat screenX, HFloat screenY);

	virtual UINT GetAlignedConstantBufferSize();
	virtual void SetCameraBuffer();

private:
	shared_ptr<DXResource> m_dxResources;

	D3D12_RECT m_scissorRect;

	HFloat4x4 m_projectionMatrix;
	HFloat4x4 m_viewMatrix;

	HFloat m_fovY;
	HFloat m_nearZ, m_farZ;
	HFloat3 m_at, m_up;

	CBufferEyePos		m_cbEyePos;
};

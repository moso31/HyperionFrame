#include "Camera.h"
#include "PipelineManager.h"
#include "DirectXHelper.h"

Camera::Camera()
{
}

Camera::Camera(const shared_ptr<DXResource>& dxResources) :
	m_dxResources(dxResources)
{
	SetRotation(0.0f, 0.0f, 0.0f);
}

Camera::~Camera()
{
}

void Camera::Init(HFloat fovY, HFloat nearZ, HFloat farZ)
{
	HFloat2 outputSize = m_dxResources->GetOutputSize();
	HFloat aspectRatio = outputSize.x / outputSize.y;
	HFloat fovAngleY = fovY * H_PI / 180.0f;

	m_fovY = fovAngleY;
	m_nearZ = nearZ;
	m_farZ = farZ;

	D3D12_VIEWPORT viewport = m_dxResources->GetScreenViewport();
	m_scissorRect = { 0, 0, static_cast<LONG>(viewport.Width), static_cast<LONG>(viewport.Height) };

	m_projectionMatrix.SetPerspLH(fovAngleY, aspectRatio, nearZ, farZ);

	PipelineManager::s_constantBufferData.projection = m_projectionMatrix.Transpose();
}

void Camera::OnResize()
{
}

void Camera::Update()
{
	PipelineManager::s_constantBufferData.view = m_viewMatrix.Transpose();

	m_cbEyePos.eyePos = translation;
	memcpy(m_mappedConstantBuffer, &m_cbEyePos, sizeof(m_cbEyePos));
}

void Camera::Render()
{
}

void Camera::SetTranslation(HFloat x, HFloat y, HFloat z)
{
	translation = { x, y, z };

	HFloat4x4 mxR;
	mxR.SetRotationXYZ(rotation);
	HFloat3 dir = HFloat4(0.0f, 0.0f, 1.0f, 0.0f).Transform(mxR);

	m_at = translation + dir;
	m_up = { 0.0f, 1.0f, 0.0f };

	m_viewMatrix.SetLookAtLH(translation, m_at, m_up);
}

void Camera::SetRotation(HFloat x, HFloat y, HFloat z)
{
	rotation = { x, y, z };

	HFloat4x4 mxR;
	mxR.SetRotationXYZ(rotation);
	HFloat3 dir = HFloat4(0.0f, 0.0f, 1.0f, 0.0f).Transform(mxR);

	m_at = translation + dir;
	m_up = { 0.0f, 1.0f, 0.0f };

	m_viewMatrix.SetLookAtLH(translation, m_at, m_up);
}

void Camera::SetLookAt(HFloat x, HFloat y, HFloat z)
{
	m_at = { x, y, z };
	m_up = { 0.0f, 1.0f, 0.0f };

	HFloat3 dir = m_at - translation;
	HFloat dis = sqrtf(dir.x * dir.x + dir.z * dir.z);

	rotation = HFloat3(
		atan2f(-dir.y, dis),	// pitch
		atan2f(dir.x, dir.z),	// yaw
		0.0f				// roll
	);

	m_viewMatrix.SetLookAtLH(translation, m_at, m_up);
}

HFloat3 Camera::GetForward()
{
	return (m_at - translation).Normalize();
}

HFloat3 Camera::GetLeft()
{
	return (m_at - translation).Cross(m_up).Normalize();
}

HFloat3 Camera::GetAt()
{
	return m_at;
}

HFloat3 Camera::GetUp()
{
	return m_up;
}

HFloat4x4 Camera::GetViewToWorld(HFloat4x4& out_mxResult)
{
	HFloat3 dir = (m_at - translation).Normalize();
	HFloat3 left = dir.Cross(m_up).Normalize();
	HFloat3 newUp = dir.Cross(left);

	out_mxResult.v[0] = HFloat4(left, 0.0f);
	out_mxResult.v[1] = HFloat4(newUp, 0.0f);
	out_mxResult.v[2] = HFloat4(dir, 0.0f);
	out_mxResult.v[3] = HFloat4(translation, 1.0f);

	return out_mxResult.Transpose();
}

Ray Camera::GenerateRay(HFloat screenX, HFloat screenY)
{
	HFloat2 outputSize = m_dxResources->GetOutputSize();
	HFloat x = (2.0f * screenX / outputSize.x - 1.0f) / m_projectionMatrix._11;
	HFloat y = (1.0f - 2.0f * screenY / outputSize.y) / m_projectionMatrix._22;

	HFloat3 vOrig(0.0f);
	HFloat3 vDir(-x, y, 1.0f);

	HFloat4x4 mxR;
	mxR.SetRotationXYZ(rotation);

	HFloat3 vOrigWorld = vOrig.TransformCoord(m_viewMatrix.Inverse());
	HFloat3 vDirWorld = vDir.TransformNormal(mxR);

	return Ray(vOrigWorld, vDirWorld);
}

UINT Camera::GetAlignedConstantBufferSize()
{
	return (UINT)(sizeof(CBufferEyePos) + 255) & ~255;
}

void Camera::SetCameraBuffer()
{
	auto pD3DDevice = m_dxResources->GetD3DDevice();

	CD3DX12_HEAP_PROPERTIES uploadHeapProperties(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC constantBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(DXResource::c_frameCount * GetAlignedConstantBufferSize());
	DX::ThrowIfFailed(pD3DDevice->CreateCommittedResource(
		&uploadHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&constantBufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_constantBuffer)));

	DX::NAME_D3D12_OBJECT(m_constantBuffer);

	// 映射常量缓冲区。
	CD3DX12_RANGE readRange(0, 0);		// 我们不打算从 CPU 上的此资源中进行读取。
	DX::ThrowIfFailed(m_constantBuffer->Map(0, &readRange, reinterpret_cast<void**>(&m_mappedConstantBuffer)));
	ZeroMemory(m_mappedConstantBuffer, DXResource::c_frameCount * GetAlignedConstantBufferSize());
	// 应用关闭之前，我们不会对此取消映射。在资源生命周期内使对象保持映射状态是可行的。
}

#include "Camera.h"
#include "PipelineManager.h"

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

void Camera::Init(float m_nearZ, float m_farZ)
{
	XMFLOAT2 outputSize = m_dxResources->GetOutputSize();
	float aspectRatio = outputSize.x / outputSize.y;
	float fovAngleY = 70.0f * XM_PI / 180.0f;

	D3D12_VIEWPORT viewport = m_dxResources->GetScreenViewport();
	m_scissorRect = { 0, 0, static_cast<LONG>(viewport.Width), static_cast<LONG>(viewport.Height) };

	// 此示例使用行主序矩阵利用右手坐标系。
	XMMATRIX projectionMatrix =
		XMMatrixTranspose(XMMatrixPerspectiveFovRH(
			fovAngleY,
			aspectRatio,
			m_nearZ,
			m_farZ
		));
	XMStoreFloat4x4(&m_projectionMatrix, projectionMatrix);
	XMStoreFloat4x4(&PipelineManager::s_constantBufferData.projection, projectionMatrix);
}

void Camera::OnResize()
{
}

void Camera::Update()
{
	XMStoreFloat4x4(&PipelineManager::s_constantBufferData.view, XMLoadFloat4x4(&m_viewMatrix));
}

void Camera::Render()
{
}

void Camera::SetRotation(float x, float y, float z)
{
	rotation = { x, y, z };

	XMVECTOR eye = XMLoadFloat3(&translation);
	XMVECTOR dir = XMVector4Transform(
		XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),
		XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&rotation))
	);

	XMVECTOR at = eye + dir;
	XMVECTOR up = { 0.0f, 1.0f, 0.0f, 0.0f };

	XMStoreFloat3(&m_at, at);
	XMStoreFloat3(&m_up, up);

	XMMATRIX viewMatrix = XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up));
	XMStoreFloat4x4(&m_viewMatrix, viewMatrix);
}

void Camera::SetLookAt(float x, float y, float z)
{
	XMFLOAT3 at = { x, y, z };

	XMVECTOR vEye = XMLoadFloat3(&translation);
	XMVECTOR vAt = XMLoadFloat3(&at);
	XMVECTOR vDir = vAt - vEye;

	XMFLOAT3 dir;
	XMStoreFloat3(&dir, vDir);
	float dis = dir.x * dir.x + dir.z * dir.z;

	rotation = XMFLOAT3(
		atan2f(-dir.y, dis),	// pitch
		atan2f(dir.x, dir.z),	// yaw
		0.0f				// roll
	);

	XMVECTOR vUp = { 0.0f, 1.0f, 0.0f, 0.0f };

	XMStoreFloat3(&m_at, vAt);
	XMStoreFloat3(&m_up, vUp);

	XMMATRIX viewMatrix = XMMatrixTranspose(XMMatrixLookAtRH(vEye, vAt, vUp));
	XMStoreFloat4x4(&m_viewMatrix, viewMatrix);
}

XMFLOAT3 Camera::GetAt()
{
	return m_at;
}

XMFLOAT3 Camera::GetUp()
{
	return m_up;
}

Ray Camera::GenerateRay(float screenX, float screenY)
{
	float x = (2.0f * (float)screenX / (float)m_dxResources->GetOutputSize().x - 1.0f) / m_projectionMatrix._11;
	float y = (1.0f - 2.0f * (float)screenX / (float)m_dxResources->GetOutputSize().y) / m_projectionMatrix._22;
	XMVECTOR vOrigin = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR vDir = XMVectorSet(x, y, 1.0f, 0.0f);

	XMMATRIX mxView = XMLoadFloat4x4(&m_viewMatrix);
	XMMATRIX mxInvView = XMMatrixInverse(&XMMatrixDeterminant(mxView), mxView);

	XMVECTOR vWorldOrigin = XMVector3TransformCoord(vOrigin, mxInvView);
	XMVECTOR vWorldDir = XMVector3TransformNormal(vDir, mxInvView);

	XMFLOAT3 origin, dir;
	XMStoreFloat3(&origin, vWorldOrigin);
	XMStoreFloat3(&dir, vWorldDir);

	Ray result(origin, dir);
	return result;
}

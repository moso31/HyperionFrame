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

void Camera::Init(float fovY, float nearZ, float farZ)
{
	XMFLOAT2 outputSize = m_dxResources->GetOutputSize();
	float aspectRatio = outputSize.x / outputSize.y;
	float fovAngleY = fovY * XM_PI / 180.0f;

	m_fovY = fovAngleY;
	m_nearZ = nearZ;
	m_farZ = farZ;

	D3D12_VIEWPORT viewport = m_dxResources->GetScreenViewport();
	m_scissorRect = { 0, 0, static_cast<LONG>(viewport.Width), static_cast<LONG>(viewport.Height) };

	// 此示例使用行主序矩阵利用右手坐标系。
	XMMATRIX projectionMatrix =
		XMMatrixPerspectiveFovRH(
			fovAngleY,
			aspectRatio,
			nearZ,
			farZ
		);
	XMStoreFloat4x4(&m_projectionMatrix, projectionMatrix);
	XMStoreFloat4x4(&PipelineManager::s_constantBufferData.projection, XMMatrixTranspose(projectionMatrix));
}

void Camera::OnResize()
{
}

void Camera::Update()
{
	XMStoreFloat4x4(&PipelineManager::s_constantBufferData.view, XMMatrixTranspose(XMLoadFloat4x4(&m_viewMatrix)));
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

	XMMATRIX viewMatrix = XMMatrixLookAtRH(eye, at, up);
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
	float dis = sqrtf(dir.x * dir.x + dir.z * dir.z);

	rotation = XMFLOAT3(
		atan2f(-dir.y, dis),	// pitch
		atan2f(dir.x, dir.z),	// yaw
		0.0f				// roll
	);

	XMVECTOR vUp = { 0.0f, 1.0f, 0.0f, 0.0f };

	XMStoreFloat3(&m_at, vAt);
	XMStoreFloat3(&m_up, vUp);

	XMMATRIX viewMatrix = XMMatrixLookAtRH(vEye, vAt, vUp);
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

XMFLOAT4X4 Camera::GetViewToWorld(XMMATRIX& out_mxResult)
{
	XMVECTOR vTranslation = XMLoadFloat3(&translation);
	XMVECTOR vDir = XMVector3Normalize(XMLoadFloat3(&m_at) - vTranslation);
	XMVECTOR vUp = XMVector3Normalize(XMLoadFloat3(&m_up));
	XMVECTOR vLeft = XMVector3Normalize(XMVector3Cross(vDir, vUp));
	XMVECTOR vNewUp = XMVector3Cross(vDir, vLeft);

	out_mxResult.r[0] = vLeft;
	out_mxResult.r[1] = vNewUp;
	out_mxResult.r[2] = vDir;	
	out_mxResult.r[3] = XMVectorSetW(vTranslation, 1.0f);

	XMFLOAT4X4 result;
	XMStoreFloat4x4(&result, XMMatrixTranspose(out_mxResult));
	return result;
}

Ray Camera::GenerateRay(float screenX, float screenY)
{
	XMFLOAT2 outputSize = m_dxResources->GetOutputSize();

	float x = (2.0f * screenX / outputSize.x - 1.0f) / m_projectionMatrix._11;
	float y = (1.0f - 2.0f * screenY / outputSize.y) / m_projectionMatrix._22;

	XMVECTOR vOrigin = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR vDir = XMVector3Normalize(XMVectorSet(-x, y, 1.0f, 0.0f));

	XMMATRIX mxWorld2View = XMLoadFloat4x4(&m_viewMatrix);
	XMMATRIX mxView2World = XMMatrixInverse(&XMMatrixDeterminant(mxWorld2View), mxWorld2View);

	XMMATRIX mxRotation = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	//mxRotation = XMMatrixInverse(&XMMatrixDeterminant(mxRotation), mxRotation);

	XMVECTOR vWorldOrigin = XMVector3TransformCoord(vOrigin, mxView2World);
	XMVECTOR vWorldDir = (XMVector3TransformNormal(vDir, mxRotation));

	XMFLOAT3 origin, dir;
	XMStoreFloat3(&origin, vWorldOrigin);
	XMStoreFloat3(&dir, vWorldDir);
	//XMStoreFloat3(&dir, XMVectorSetZ(vWorldDir, -XMVectorGetZ(vWorldDir)));

	Ray result(origin, dir);
	return result;
}

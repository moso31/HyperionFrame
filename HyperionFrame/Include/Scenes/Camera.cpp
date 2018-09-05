#include "Camera.h"
#include "PipelineManager.h"

Camera::Camera()
{
}

Camera::Camera(const shared_ptr<DXResource>& dxResources) :
	m_dxResources(dxResources)
{
}

Camera::~Camera()
{
}

void Camera::Init()
{
	XMFLOAT2 outputSize = m_dxResources->GetOutputSize();
	float aspectRatio = outputSize.x / outputSize.y;
	float fovAngleY = 70.0f * XM_PI / 180.0f;

	D3D12_VIEWPORT viewport = m_dxResources->GetScreenViewport();
	m_scissorRect = { 0, 0, static_cast<LONG>(viewport.Width), static_cast<LONG>(viewport.Height) };

	// ��ʾ��ʹ�����������������������ϵ��
	XMStoreFloat4x4(&PipelineManager::s_constantBufferData.projection,
		XMMatrixTranspose(XMMatrixPerspectiveFovRH(
			fovAngleY,
			aspectRatio,
			0.01f,
			100.0f
		)));

	// �۾�λ��(0,0.7,1.5)�������� Y ��ʹ������ʸ�����ҵ�(0,-0.1,0)��
	static const XMVECTORF32 eye = { 0.0f, 0.7f, 1.5f, 0.0f };
	static const XMVECTORF32 at = { 0.0f, -0.1f, 0.0f, 0.0f };
	static const XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

	XMStoreFloat4x4(&PipelineManager::s_constantBufferData.view, XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));
}

void Camera::OnResize()
{
}

void Camera::Update()
{
}

void Camera::Render()
{
}

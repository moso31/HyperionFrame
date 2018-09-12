#pragma once
#include "Transform.h"
#include "Camera.h"

class Shape : public Transform
{
public:
	Shape();
	Shape(const shared_ptr<DXResource>& dxResources, Camera* camera);
	~Shape();

protected:
	Camera*		m_camera;

	shared_ptr<DXResource> m_dxResources;

	// ���弸�ε� Direct3D ��Դ��
	ComPtr<ID3D12Resource>								m_vertexBuffer;
	ComPtr<ID3D12Resource>								m_indexBuffer;

	// �� GPU ��Ĭ�϶��д������㻺������Դ��ʹ�����ضѽ��������ݸ��Ƶ����С�
	// �� GPU ʹ����֮ǰ�������ͷ�������Դ��
	ComPtr<ID3D12Resource> m_vertexBufferUpload;
	ComPtr<ID3D12Resource> m_indexBufferUpload;

	D3D12_VERTEX_BUFFER_VIEW							m_vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW								m_indexBufferView;
};

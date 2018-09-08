#pragma once
#include "Camera.h"

class Box : public Shape
{
public:
	static const UINT c_alignedConstantBufferSize = (sizeof(ModelViewProjectionConstantBuffer) + 255) & ~255; // ������������С���붼�� 256 �ֽڵ���������

	Box();
	Box(const shared_ptr<DXResource>& dxResources, Camera* camera);
	~Box();

	void Init(ComPtr<ID3D12GraphicsCommandList> pCommandList);

	void Update(UINT8 * destination);
	void Render(ComPtr<ID3D12GraphicsCommandList> pCommandList);

	void ReleaseUploadBuffers();

private:
	Camera* m_camera;
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

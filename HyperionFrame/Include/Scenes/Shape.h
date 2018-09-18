#pragma once
#include "Camera.h"

struct HTriangle
{
	union 
	{
		struct
		{
			XMFLOAT3 point0, point1, point2;
		};
		struct
		{
			XMFLOAT3 p[3];
		};
	};
};

class Shape : public Transform
{
public:
	Shape();
	Shape(const shared_ptr<DXResource>& dxResources, Camera* camera);
	~Shape();

	AABB GetAABB();

	// ���¼�������ģ�͵�AABB��
	AABB GenerateAABB();

	virtual void Init(ComPtr<ID3D12GraphicsCommandList> pCommandList) = 0;
	virtual void Update(UINT8 * destination) = 0;
	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pCommandList) = 0;

	UINT GetFaceCount();
	HTriangle GetFace(UINT faceIndex);

private:
	// ��ʼ������
	virtual void _initBufferData(ComPtr<ID3D12GraphicsCommandList> pCommandList) = 0;

	// ��ʼ�����ݣ�AABB������ߡ��뾶�ȣ�
	virtual void _initParameters() = 0;

protected:
	Camera*		m_camera;
	AABB		m_aabb;

	shared_ptr<DXResource> m_dxResources;

	// ���弸�ε� Direct3D ��Դ��
	ComPtr<ID3D12Resource>			m_vertexBuffer;
	ComPtr<ID3D12Resource>			m_indexBuffer;

	// �� GPU ��Ĭ�϶��д������㻺������Դ��ʹ�����ضѽ��������ݸ��Ƶ����С�
	// �� GPU ʹ����֮ǰ�������ͷ�������Դ��
	ComPtr<ID3D12Resource>			m_vertexBufferUpload;
	ComPtr<ID3D12Resource>			m_indexBufferUpload;

	D3D12_VERTEX_BUFFER_VIEW		m_vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW			m_indexBufferView;

	vector<VertexPositionColor>		m_vertices;
	vector<USHORT>					m_indices;
};

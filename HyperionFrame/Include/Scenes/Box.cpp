#include "Box.h"
#include "PipelineManager.h"

Box::Box()
{
}

Box::Box(const shared_ptr<DXResource>& dxResources, Camera* camera) :
	Shape(dxResources, camera)
{
}

Box::~Box()
{
}

void Box::Init(ComPtr<ID3D12GraphicsCommandList> pCommandList)
{
	_initBufferData(pCommandList);
	_initParameters();
}

void Box::Update(UINT8* destination)
{
	// 准备将更新的模型矩阵传递到着色器。
	XMStoreFloat4x4(&PipelineManager::s_constantBufferData.model, XMMatrixTranspose(XMLoadFloat4x4(&GetWorldMatrix())));

	memcpy(destination, &PipelineManager::s_constantBufferData, sizeof(PipelineManager::s_constantBufferData));
}

void Box::Render(ComPtr<ID3D12GraphicsCommandList> pCommandList)
{
	pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	pCommandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
	pCommandList->IASetIndexBuffer(&m_indexBufferView);
	pCommandList->DrawIndexedInstanced(36, 1, 0, 0, 0);
}

void Box::ReleaseUploadBuffers()
{
}

void Box::Intersect(Ray worldRay, XMFLOAT3 & out_hitPos, int & out_hitIndex, SurfaceInteraction* out_isect)
{
	XMVECTOR vMax = XMLoadFloat3(&m_aabb.GetVecMax());
	XMVECTOR vMin = XMLoadFloat3(&m_aabb.GetVecMin());

	XMMATRIX mxObject2World = XMLoadFloat4x4(&GetWorldMatrix());
	XMMATRIX mxWorld2Object = XMMatrixInverse(&XMMatrixDeterminant(mxObject2World), mxObject2World);
	XMVECTOR vRayOrig = XMVector3TransformCoord(XMLoadFloat3(&worldRay.GetOrigin()), mxWorld2Object);
	XMVECTOR vRayDir = XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&worldRay.GetDirection()), mxWorld2Object));
	XMVECTOR vRayDirReciprocal = XMVectorReciprocal(vRayDir);

	XMVECTOR tMax = (vMax - vRayOrig) * vRayDirReciprocal;
	XMVECTOR tMin = (vMin - vRayOrig) * vRayDirReciprocal;

	XMFLOAT3 t1, t2;
	XMStoreFloat3(&t1, XMVectorMin(tMin, tMax));
	XMStoreFloat3(&t2, XMVectorMax(tMin, tMax));

	float tNear = max(t1.x, max(t1.y, t1.z));
	float tFar = min(t2.x, min(t2.y, t2.z));

	out_hitIndex = -1;
	if (tNear < tFar)
	{
		XMStoreFloat3(&out_hitPos, vRayOrig + vRayDir * tNear);

		float nearest = FLT_MAX;
		for (UINT i = 0; i < GetFaceCount(); i++)
		{
			HTriangle triData = GetFace(i);

			XMVECTOR v0 = XMLoadFloat3(&triData.point0);
			XMVECTOR v1 = XMLoadFloat3(&triData.point1);
			XMVECTOR v2 = XMLoadFloat3(&triData.point2);

			// compute plane's normal
			XMVECTOR v0v1 = v1 - v0;
			XMVECTOR v0v2 = v2 - v0;
			// no need to normalize
			XMVECTOR N = XMVector3Cross(v0v1, v0v2); // N 
			//float area2 = N.length();

			// Step 1: finding P

			// check if ray and plane are parallel ?
			float NdotRayDirection = XMVectorGetX(XMVector3Dot(N, vRayDir));
			if (fabsf(NdotRayDirection) < 0.000001) // almost 0 
				continue; // they are parallel so they don't intersect ! 

			// compute d parameter using equation 2
			float d = XMVectorGetX(XMVector3Dot(N, v0));

			// compute t (equation 3)
			float t = -(XMVectorGetX(XMVector3Dot(N, vRayOrig)) + d) / NdotRayDirection;
			// check if the triangle is in behind the ray
			if (t < 0)
				continue; // the triangle is behind 

			// compute the intersection point using equation 1
			XMVECTOR P = vRayOrig + t * vRayDir;

			// Step 2: inside-outside test
			XMVECTOR C; // vector perpendicular to triangle's plane 

			// edge 0
			XMVECTOR edge0 = v1 - v0;
			XMVECTOR vp0 = P - v0;
			C = XMVector3Cross(edge0, vp0);

			if (XMVectorGetX(XMVector3Dot(N, C)) < 0) continue; // P is on the right side 

			// edge 1
			XMVECTOR edge1 = v2 - v1;
			XMVECTOR vp1 = P - v1;
			C = XMVector3Cross(edge1, vp1);
			if (XMVectorGetX(XMVector3Dot(N, C)) < 0) continue; // P is on the right side 

			// edge 2
			XMVECTOR edge2 = v0 - v2;
			XMVECTOR vp2 = P - v2;
			C = XMVector3Cross(edge2, vp2);
			if (XMVectorGetX(XMVector3Dot(N, C)) < 0) continue; // P is on the right side; 

			if (nearest > t)
			{
				nearest = t;
				out_hitIndex = i;
				XMStoreFloat3(&out_isect->n, N);
				XMStoreFloat3(&out_isect->wo, -vRayDir);
				out_isect->shape = this;
			}
		}
	}
}

bool Box::IntersectP(Ray worldRay)
{
	XMVECTOR vMax = XMLoadFloat3(&m_aabb.GetVecMax());
	XMVECTOR vMin = XMLoadFloat3(&m_aabb.GetVecMin());

	XMMATRIX mxObject2World = XMLoadFloat4x4(&GetWorldMatrix());
	XMMATRIX mxWorld2Object = XMMatrixInverse(&XMMatrixDeterminant(mxObject2World), mxObject2World);
	XMVECTOR vRayOrig = XMVector3TransformCoord(XMLoadFloat3(&worldRay.GetOrigin()), mxWorld2Object);
	XMVECTOR vRayDir = XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&worldRay.GetDirection()), mxWorld2Object));
	XMVECTOR vRayDirReciprocal = XMVectorReciprocal(vRayDir);

	XMVECTOR tMax = (vMax - vRayOrig) * vRayDirReciprocal;
	XMVECTOR tMin = (vMin - vRayOrig) * vRayDirReciprocal;

	XMFLOAT3 t1, t2;
	XMStoreFloat3(&t1, XMVectorMin(tMin, tMax));
	XMStoreFloat3(&t2, XMVectorMax(tMin, tMax));

	float tNear = max(t1.x, max(t1.y, t1.z));
	float tFar = min(t2.x, min(t2.y, t2.z));

	return tNear < tFar;
}

void Box::_initBufferData(ComPtr<ID3D12GraphicsCommandList> pCommandList)
{
	auto d3dDevice = m_dxResources->GetD3DDevice();

	// 创建立方体几何图形资源并上载到 GPU。

	// 立方体顶点。每个顶点都有一个位置和一个颜色。
	m_vertices =
	{
		{ XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(-0.5f, -0.5f,  0.5f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-0.5f,  0.5f, -0.5f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ XMFLOAT3(-0.5f,  0.5f,  0.5f), XMFLOAT3(0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(0.5f, -0.5f,  0.5f), XMFLOAT3(1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(0.5f,  0.5f, -0.5f), XMFLOAT3(1.0f, 1.0f, 0.0f) },
		{ XMFLOAT3(0.5f,  0.5f,  0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f) },
	};

	const UINT vertexBufferSize = UINT(sizeof(VertexPositionColor) * m_vertices.size());

	CD3DX12_HEAP_PROPERTIES defaultHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
	CD3DX12_RESOURCE_DESC vertexBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);
	ThrowIfFailed(d3dDevice->CreateCommittedResource(
		&defaultHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&vertexBufferDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&m_vertexBuffer)));

	CD3DX12_HEAP_PROPERTIES uploadHeapProperties(D3D12_HEAP_TYPE_UPLOAD);
	ThrowIfFailed(d3dDevice->CreateCommittedResource(
		&uploadHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&vertexBufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_vertexBufferUpload)));

	NAME_D3D12_OBJECT(m_vertexBuffer);
	NAME_D3D12_OBJECT(m_vertexBufferUpload);

	// 将顶点缓冲区上载到 GPU。
	{
		D3D12_SUBRESOURCE_DATA vertexData = {};
		vertexData.pData = reinterpret_cast<BYTE*>(m_vertices.data());
		vertexData.RowPitch = vertexBufferSize;
		vertexData.SlicePitch = vertexData.RowPitch;

		UpdateSubresources(pCommandList.Get(), m_vertexBuffer.Get(), m_vertexBufferUpload.Get(), 0, 0, 1, &vertexData);

		CD3DX12_RESOURCE_BARRIER vertexBufferResourceBarrier =
			CD3DX12_RESOURCE_BARRIER::Transition(m_vertexBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
		pCommandList->ResourceBarrier(1, &vertexBufferResourceBarrier);
	}

	// 加载网格索引。每三个索引表示要在屏幕上呈现的三角形。
	// 例如: 0,2,1 表示顶点缓冲区中的索引为 0、2 和 1 的顶点构成
	// 此网格的第一个三角形。
	m_indices =
	{
		0, 2, 1, // -x
		1, 2, 3,

		4, 5, 6, // +x
		5, 7, 6,

		0, 1, 5, // -y
		0, 5, 4,

		2, 6, 7, // +y
		2, 7, 3,

		0, 4, 6, // -z
		0, 6, 2,

		1, 3, 7, // +z
		1, 7, 5,
	};

	const UINT indexBufferSize = UINT(sizeof(unsigned short) * m_indices.size());

	CD3DX12_RESOURCE_DESC indexBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize);
	ThrowIfFailed(d3dDevice->CreateCommittedResource(
		&defaultHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&indexBufferDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&m_indexBuffer)));

	ThrowIfFailed(d3dDevice->CreateCommittedResource(
		&uploadHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&indexBufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_indexBufferUpload)));

	NAME_D3D12_OBJECT(m_indexBuffer);
	NAME_D3D12_OBJECT(m_indexBufferUpload);

	// 将索引缓冲区上载到 GPU。
	{
		D3D12_SUBRESOURCE_DATA indexData = {};
		indexData.pData = reinterpret_cast<BYTE*>(m_indices.data());
		indexData.RowPitch = indexBufferSize;
		indexData.SlicePitch = indexData.RowPitch;

		UpdateSubresources(pCommandList.Get(), m_indexBuffer.Get(), m_indexBufferUpload.Get(), 0, 0, 1, &indexData);

		CD3DX12_RESOURCE_BARRIER indexBufferResourceBarrier =
			CD3DX12_RESOURCE_BARRIER::Transition(m_indexBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_INDEX_BUFFER);
		pCommandList->ResourceBarrier(1, &indexBufferResourceBarrier);
	}

	// 创建顶点/索引缓冲区视图。
	m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
	m_vertexBufferView.StrideInBytes = sizeof(VertexPositionColor);
	m_vertexBufferView.SizeInBytes = (UINT)(sizeof(VertexPositionColor) * m_vertices.size());

	m_indexBufferView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
	m_indexBufferView.SizeInBytes = (UINT)(sizeof(USHORT) * m_indices.size());
	m_indexBufferView.Format = DXGI_FORMAT_R16_UINT;
}

void Box::_initParameters()
{
	m_aabb = AABB(XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(0.5f, 0.5f, 0.5f));
}

#include "Sphere.h"
#include "PipelineManager.h"
#include "Interaction.h"

Sphere::Sphere()
{
}

Sphere::Sphere(const shared_ptr<DXResource>& dxResources) :
	Shape(dxResources)
{
}

Sphere::~Sphere()
{
}

void Sphere::Init(ComPtr<ID3D12GraphicsCommandList> pCommandList, float radius, int segmentVertical, int segmentHorizontal)
{
	_initParameters(radius, segmentVertical, segmentHorizontal);
	_initBufferData(pCommandList);
}

void Sphere::Update(UINT8* destination)
{
	// 准备将更新的模型矩阵传递到着色器。
	XMStoreFloat4x4(&PipelineManager::s_constantBufferData.model, XMMatrixTranspose(XMLoadFloat4x4(&GetObject2World())));

	memcpy(destination, &PipelineManager::s_constantBufferData, sizeof(PipelineManager::s_constantBufferData));
}

void Sphere::Render(ComPtr<ID3D12GraphicsCommandList> pCommandList)
{
	pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	pCommandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
	pCommandList->IASetIndexBuffer(&m_indexBufferView);
	pCommandList->DrawIndexedInstanced((UINT)m_indices.size(), 1, 0, 0, 0);
}

void Sphere::Intersect(Ray worldRay, SurfaceInteraction* out_isect)
{
	XMMATRIX mxObject2World = XMLoadFloat4x4(&GetObject2World());
	XMMATRIX mxWorld2Object = XMLoadFloat4x4(&GetWorld2Object());

	XMFLOAT3 ro, rd;
	XMStoreFloat3(&ro, XMVector3TransformCoord(XMLoadFloat3(&worldRay.GetOrigin()), mxWorld2Object));
	XMStoreFloat3(&rd, XMVector3TransformNormal(XMLoadFloat3(&worldRay.GetDirection()), mxWorld2Object));
	Ray ray(ro, rd);

	float a = rd.x * rd.x + rd.y * rd.y + rd.z * rd.z;
	float b = 2.0f * (rd.x * ro.x + rd.y * ro.y + rd.z * ro.z);
	float c = ro.x * ro.x + ro.y * ro.y + ro.z * ro.z - m_radius * m_radius;

	float t0, t1;
	if (!Quadratic(a, b, c, t0, t1)) 
		return;

	if (t1 < H_EPSILON)
		return;
	float tShapeHit = t0;
	if (tShapeHit <= 0.0f) 
		tShapeHit = t1;

	XMFLOAT3 pHit = ray.GetT(tShapeHit);
	XMVECTOR pHitV = XMLoadFloat3(&pHit);
	pHitV *= XMVectorReplicate(m_radius) / XMVector3Length(pHitV);
	XMStoreFloat3(&pHit, pHitV);
	
	float phi = atan2(XMVectorGetY(pHitV), XMVectorGetX(pHitV));
	if (phi < 0.0f) phi += H_2PI;

	float u = phi / H_2PI;
	float theta = acos(Clamp(pHit.z / m_radius, -1.0f, 1.0f));
	float v = theta / H_PI;

	float zRadius = std::sqrt(pHit.x * pHit.x + pHit.y * pHit.y);
	float invZRadius = 1.0f / zRadius;
	float cosPhi = pHit.x * invZRadius;
	float sinPhi = pHit.y * invZRadius;
	XMFLOAT3 dpdu(-H_2PI * pHit.y, H_2PI * pHit.x, 0.0f);
	XMFLOAT3 dpdv = XMFLOAT3(H_PI * pHit.z * cosPhi, H_PI * pHit.z * sinPhi, H_PI * -m_radius * std::sin(theta));

	{
		XMVECTOR pV = XMVector3TransformCoord(XMLoadFloat3(&pHit), mxObject2World);
		XMVECTOR woV = XMVector3Normalize(XMVector3TransformNormal(-XMLoadFloat3(&rd), mxObject2World));
		XMVECTOR dpduV = XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&dpdu), mxObject2World));
		XMVECTOR dpdvV = XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&dpdv), mxObject2World));
		XMVECTOR nV = XMVector3Normalize(XMVector3Cross(dpdvV, dpduV));

		SurfaceInteraction isect;
		XMStoreFloat3(&isect.p, pV);
		XMStoreFloat3(&isect.n, nV);
		XMStoreFloat3(&isect.wo, woV);
		XMStoreFloat3(&isect.dpdu, dpduV);
		XMStoreFloat3(&isect.dpdv, dpdvV);
		isect.uv = XMFLOAT2(u, v);
		isect.shape = this;

		*out_isect = isect;
	}
}

bool Sphere::IntersectP(Ray worldRay)
{
	XMFLOAT3 ro, rd;
	XMStoreFloat3(&ro, XMVector3TransformCoord(XMLoadFloat3(&worldRay.GetOrigin()), XMLoadFloat4x4(&GetWorld2Object())));
	XMStoreFloat3(&rd, XMVector3TransformNormal(XMLoadFloat3(&worldRay.GetDirection()), XMLoadFloat4x4(&GetWorld2Object())));
	Ray ray(ro, rd);

	float a = rd.x * rd.x + rd.y * rd.y + rd.z * rd.z;
	float b = 2 * (rd.x * ro.x + rd.y * ro.y + rd.z * ro.z);
	float c = ro.x * ro.x + ro.y * ro.y + ro.z * ro.z - m_radius * m_radius;

	float t0, t1;
	if (!Quadratic(a, b, c, t0, t1)) return false;

	return t1 > H_EPSILON;
}

void Sphere::_initBufferData(ComPtr<ID3D12GraphicsCommandList> pCommandList)
{
	auto d3dDevice = m_dxResources->GetD3DDevice();

	const UINT vertexBufferSize = UINT(sizeof(VertexPCT) * m_vertices.size());

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
	m_vertexBufferView.StrideInBytes = sizeof(VertexPCT);
	m_vertexBufferView.SizeInBytes = (UINT)(sizeof(VertexPCT) * m_vertices.size());

	m_indexBufferView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
	m_indexBufferView.SizeInBytes = (UINT)(sizeof(USHORT) * m_indices.size());
	m_indexBufferView.Format = DXGI_FORMAT_R16_UINT;
}

void Sphere::_initParameters(float radius, int segmentVertical, int segmentHorizontal)
{
	int currVertIdx = 0;
	for (int i = 0; i < segmentVertical; i++)
	{
		float yDown = ((float)i / (float)segmentVertical * 2.0f - 1.0f) * radius;
		float yUp = ((float)(i + 1) / (float)segmentVertical * 2.0f - 1.0f) * radius;
		float radiusDown = sqrtf(1.0f - yDown * yDown);
		float radiusUp = sqrtf(1.0f - yUp * yUp);

		for (int j = 0; j < segmentHorizontal; j++)
		{
			float segNow = (float)j / (float)segmentHorizontal;
			float segNext = (float)(j + 1) / (float)segmentHorizontal;
			float angleNow = segNow * XM_2PI;
			float angleNext = segNext * XM_2PI;
			float xNow = sin(angleNow);
			float zNow = cos(angleNow);
			float xNext = sin(angleNext);
			float zNext = cos(angleNext);

			XMFLOAT3 pNowUp = { xNow * radiusUp, yUp, zNow * radiusUp };
			XMFLOAT3 pNextUp = { xNext * radiusUp, yUp, zNext * radiusUp };
			XMFLOAT3 pNowDown = { xNow * radiusDown, yDown, zNow * radiusDown };
			XMFLOAT3 pNextDown = { xNext * radiusDown, yDown, zNext * radiusDown };

			XMFLOAT2 uvNowUp = { segNow, yUp };
			XMFLOAT2 uvNextUp = { segNext, yUp };
			XMFLOAT2 uvNowDown = { segNow, yDown };
			XMFLOAT2 uvNextDown = { segNext, yDown };

			m_vertices.push_back({ pNowUp,		XMFLOAT3(0.0f, 0.0f, 0.3f), uvNowUp });
			m_vertices.push_back({ pNextUp,		XMFLOAT3(0.0f, 0.0f, 0.3f), uvNextUp });
			m_vertices.push_back({ pNextDown,	XMFLOAT3(0.0f, 0.0f, 0.3f), uvNextDown });
			m_vertices.push_back({ pNowDown,	XMFLOAT3(0.0f, 0.0f, 0.3f), uvNowDown });

			m_indices.push_back(currVertIdx);
			m_indices.push_back(currVertIdx + 1);
			m_indices.push_back(currVertIdx + 2);
			m_indices.push_back(currVertIdx);
			m_indices.push_back(currVertIdx + 2);
			m_indices.push_back(currVertIdx + 3);

			currVertIdx += 4;
		}
	}

	for (int i = 0; i < m_vertices.size(); i++)
	{
		m_aabb.Merge(m_vertices[i].pos);
	}

	m_radius = radius;
	m_segmentVertical = segmentVertical;
	m_segmentHorizontal = segmentHorizontal;
}

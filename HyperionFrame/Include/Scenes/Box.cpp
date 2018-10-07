#include "Box.h"
#include "PipelineManager.h"
#include "Interaction.h"

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

void Box::Intersect(Ray worldRay, int & out_hitIndex, SurfaceInteraction* out_isect)
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
	
	float tNearest = FLT_MAX;
	out_hitIndex = -1;
	if (tNear < tFar)
	{
		for (UINT i = 0; i < GetFaceCount(); i++)
		{
			XMFLOAT3 triData[3];
			GetFace(i, triData);

			XMVECTOR p0 = XMLoadFloat3(&triData[0]);
			XMVECTOR p1 = XMLoadFloat3(&triData[1]);
			XMVECTOR p2 = XMLoadFloat3(&triData[2]);

			XMVECTOR p0t = p0 - vRayOrig;
			XMVECTOR p1t = p1 - vRayOrig;
			XMVECTOR p2t = p2 - vRayOrig;

			int kz = 0;
			float kValue = fabsf(XMVectorGetByIndex(vRayDir, 0));
			for (int i = 1; i < 4; i++)
			{
				float getByIdx = fabsf(XMVectorGetByIndex(vRayDir, i));
				if (kValue < getByIdx)
				{
					kValue = getByIdx;
					kz = i;
				}
			}
			int kx = kz + 1;
			if (kx == 3) kx = 0;
			int ky = kx + 1;
			if (ky == 3) ky = 0;

			XMFLOAT3 d, fp0t, fp1t, fp2t;
			XMStoreFloat3(&d, XMVectorPermute(vRayDir, vRayDir, kx, ky, kz, 3));
			XMStoreFloat3(&fp0t, XMVectorPermute(p0t, p0t, kx, ky, kz, 3));
			XMStoreFloat3(&fp1t, XMVectorPermute(p1t, p1t, kx, ky, kz, 3));
			XMStoreFloat3(&fp2t, XMVectorPermute(p2t, p2t, kx, ky, kz, 3));

			float Sx = -d.x / d.z;
			float Sy = -d.y / d.z;
			float Sz = 1.f / d.z;
			fp0t.x += Sx * fp0t.z;
			fp0t.y += Sy * fp0t.z;
			fp1t.x += Sx * fp1t.z;
			fp1t.y += Sy * fp1t.z;
			fp2t.x += Sx * fp2t.z;
			fp2t.y += Sy * fp2t.z;

			float e0 = fp1t.x * fp2t.y - fp1t.y * fp2t.x;
			float e1 = fp2t.x * fp0t.y - fp2t.y * fp0t.x;
			float e2 = fp0t.x * fp1t.y - fp0t.y * fp1t.x;

			if (e0 == 0.0f || e1 == 0.0f || e2 == 0.0f) 
			{
				double p2txp1ty = (double)fp2t.x * (double)fp1t.y;
				double p2typ1tx = (double)fp2t.y * (double)fp1t.x;
				e0 = (float)(p2typ1tx - p2txp1ty);
				double p0txp2ty = (double)fp0t.x * (double)fp2t.y;
				double p0typ2tx = (double)fp0t.y * (double)fp2t.x;
				e1 = (float)(p0typ2tx - p0txp2ty);
				double p1txp0ty = (double)fp1t.x * (double)fp0t.y;
				double p1typ0tx = (double)fp1t.y * (double)fp0t.x;
				e2 = (float)(p1typ0tx - p1txp0ty);
			}

			if ((e0 < 0 || e1 < 0 || e2 < 0) && (e0 > 0 || e1 > 0 || e2 > 0))
				continue;
			float det = e0 + e1 + e2;
			if (det == 0) continue;

			fp0t.z *= Sz;
			fp1t.z *= Sz;
			fp2t.z *= Sz;
			float tScaled = e0 * fp0t.z + e1 * fp1t.z + e2 * fp2t.z;
			if (det < 0 && (tScaled >= 0/* || tScaled < ray.tMax * det*/))
				continue;
			else if (det > 0 && (tScaled <= 0/* || tScaled > ray.tMax * det*/))
				continue;

			float invDet = 1 / det;
			float b0 = e0 * invDet;
			float b1 = e1 * invDet;
			float b2 = e2 * invDet;
			float t = tScaled * invDet;

			XMFLOAT2 uv[3];
			GetUVs(i, uv);

			float du02 = uv[0].x - uv[2].x;
			float du12 = uv[1].x - uv[2].x;
			float dv02 = uv[0].y - uv[2].y;
			float dv12 = uv[1].y - uv[2].y;
			XMVECTOR dp02 = p0 - p2, dp12 = p1 - p2;
			float detUV = du02 * dv12 - dv02 * du12;
			float invdetUV = 1.0f / det;

			XMFLOAT3 dpdu, dpdv;
			XMStoreFloat3(&dpdu, dv12 * dp02 - dv02 * dp12);
			XMStoreFloat3(&dpdv, du02 * dp12 - du12 * dp02);

			XMFLOAT3 pHit;
			XMStoreFloat3(&pHit, b0 * p0 + b1 * p1 + b2 * p2);
			XMFLOAT2 uvHit;
			XMStoreFloat2(&uvHit, b0 * XMLoadFloat2(&uv[0]) + b1 * XMLoadFloat2(&uv[1]) + b2 * XMLoadFloat2(&uv[2]));

			if (tNearest > t)
			{
				tNearest = t;
				out_hitIndex = i;
				XMFLOAT3 hitPos;
				XMStoreFloat3(&hitPos, -(vRayOrig + t * vRayDir));
				
				*out_isect = SurfaceInteraction(hitPos, uvHit, hitPos, dpdu, dpdv, this);
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
		// -X
		{ XMFLOAT3(-0.5f,  0.5f,  0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-0.5f,  0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-0.5f, -0.5f,  0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		
		// +X
		{ XMFLOAT3( 0.5f,  0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3( 0.5f,  0.5f,  0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3( 0.5f, -0.5f,  0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3( 0.5f, -0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },

		// -Y
		{ XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3( 0.5f, -0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3( 0.5f, -0.5f,  0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-0.5f, -0.5f,  0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		
		// +Y
		{ XMFLOAT3(-0.5f,  0.5f,  0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3( 0.5f,  0.5f,  0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3( 0.5f,  0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-0.5f,  0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		
		// -Z
		{ XMFLOAT3( 0.5f,  0.5f,  0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-0.5f,  0.5f,  0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-0.5f, -0.5f,  0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3( 0.5f, -0.5f,  0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		
		// +Z
		{ XMFLOAT3(-0.5f,  0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3( 0.5f,  0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3( 0.5f, -0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	};

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

	// 加载网格索引。每三个索引表示要在屏幕上呈现的三角形。
	// 例如: 0,2,1 表示顶点缓冲区中的索引为 0、2 和 1 的顶点构成
	// 此网格的第一个三角形。
	m_indices =
	{
		0, 2, 1,
		0, 3, 2,

		4, 6, 5,
		4, 7, 6,

		8, 10, 9,
		8, 11, 10,

		12, 14, 13,
		12, 15, 14,

		16, 18, 17,
		16, 19, 18,

		20, 22, 21,
		20, 23, 22
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
	m_vertexBufferView.StrideInBytes = sizeof(VertexPCT);
	m_vertexBufferView.SizeInBytes = (UINT)(sizeof(VertexPCT) * m_vertices.size());

	m_indexBufferView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
	m_indexBufferView.SizeInBytes = (UINT)(sizeof(USHORT) * m_indices.size());
	m_indexBufferView.Format = DXGI_FORMAT_R16_UINT;
}

void Box::_initParameters()
{
	m_aabb = AABB(XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(0.5f, 0.5f, 0.5f));
}

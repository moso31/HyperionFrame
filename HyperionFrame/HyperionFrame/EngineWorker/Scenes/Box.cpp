#include "Box.h"
#include "PipelineManager.h"
#include "Interaction.h"

Box::Box()
{
}

Box::Box(const shared_ptr<DXResource>& dxResources) :
	Shape(dxResources)
{
	m_type = ePrimitiveType::BOX;
}

Box::~Box()
{
}

void Box::InitParameters()
{
	float x = 0.5f, y = 0.5f, z = 0.5f;
	m_vertices =
	{
		// -X
		{ XMFLOAT3(-x, +y, +z), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-x, +y, -z), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-x, -y, -z), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-x, -y, +z), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },

		// +X
		{ XMFLOAT3(+x, +y, -z), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(+x, +y, +z), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(+x, -y, +z), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(+x, -y, -z), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },

		// -Y
		{ XMFLOAT3(-x, -y, -z), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(+x, -y, -z), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(+x, -y, +z), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-x, -y, +z), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },

		// +Y
		{ XMFLOAT3(-x, +y, +z), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(+x, +y, +z), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(+x, +y, -z), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-x, +y, -z), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },

		// -Z
		{ XMFLOAT3(-x, +y, -z), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(+x, +y, -z), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(+x, -y, -z), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-x, -y, -z), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },

		// +Z
		{ XMFLOAT3(+x, +y, +z), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-x, +y, +z), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-x, -y, +z), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(+x, -y, +z), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
	};

	m_indices =
	{
		0,  2,  1,
		0,  3,  2,
		        
		4,  6,  5,
		4,  7,  6,
		    
		8,  10, 9,
		8,  11, 10,

		12, 14, 13,
		12, 15, 14,

		16, 18, 17,
		16, 19, 18,

		20, 22, 21,
		20, 23, 22
	};

	m_aabb = AABB(XMFLOAT3(-x, -y, -z), XMFLOAT3(x, y, z));
}

void Box::Update(UINT8* destination)
{
	// 准备将更新的模型矩阵传递到着色器。
	XMStoreFloat4x4(&PipelineManager::s_constantBufferData.model, XMMatrixTranspose(XMLoadFloat4x4(&GetObject2World())));

	memcpy(destination, &PipelineManager::s_constantBufferData, sizeof(PipelineManager::s_constantBufferData));
	memcpy(destination + 256, &m_cbMeshData, sizeof(m_cbMeshData));
}

void Box::Render(ComPtr<ID3D12GraphicsCommandList> pCommandList)
{
	pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	pCommandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
	pCommandList->IASetIndexBuffer(&m_indexBufferView);
	pCommandList->DrawIndexedInstanced(36, 1, 0, 0, 0);
}

bool Box::Intersect(Ray worldRay, SurfaceInteraction* out_isect, float* out_tHit)
{
	XMVECTOR vMax = XMLoadFloat3(&m_aabb.GetVecMax());
	XMVECTOR vMin = XMLoadFloat3(&m_aabb.GetVecMin());

	XMMATRIX mxObject2World = XMLoadFloat4x4(&GetObject2World());
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
	
	SurfaceInteraction record;
	*out_tHit = FLT_MAX;
	if (tNear < tFar && tNear > H_EPSILON)
	{
		for (UINT i = 0; i < GetFaceCount(); i++)
		{
			Triangle tri = GetFace(i);

			XMVECTOR p0 = XMLoadFloat3(&tri.p[0]);
			XMVECTOR p1 = XMLoadFloat3(&tri.p[1]);
			XMVECTOR p2 = XMLoadFloat3(&tri.p[2]);

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

			TriangleUV uv = GetUVs(i);

			float du02 = uv.p[0].x - uv.p[2].x;
			float du12 = uv.p[1].x - uv.p[2].x;
			float dv02 = uv.p[0].y - uv.p[2].y;
			float dv12 = uv.p[1].y - uv.p[2].y;
			XMVECTOR dp02 = p0 - p2, dp12 = p1 - p2;
			float detUV = du02 * dv12 - dv02 * du12;
			float invdetUV = 1.0f / det;

			XMFLOAT3 dpdu, dpdv;
			XMStoreFloat3(&dpdu, dv12 * dp02 - dv02 * dp12);
			XMStoreFloat3(&dpdv, du02 * dp12 - du12 * dp02);

			XMFLOAT3 pHit;
			XMStoreFloat3(&pHit, b0 * p0 + b1 * p1 + b2 * p2);
			XMFLOAT2 uvHit;
			XMStoreFloat2(&uvHit, b0 * XMLoadFloat2(&uv.p[0]) + b1 * XMLoadFloat2(&uv.p[1]) + b2 * XMLoadFloat2(&uv.p[2]));

			if (*out_tHit > t && t > 1e-5f)
			{
				*out_tHit = t;
				XMFLOAT3 hitPos, wo;
				XMStoreFloat3(&hitPos, (vRayOrig + t * vRayDir));
				XMStoreFloat3(&wo, -vRayDir);
				record = SurfaceInteraction(hitPos, uvHit, wo, dpdu, dpdv, this);
			}
		}

		if (record.shape == this)
		{
			// isect 转换成世界坐标
			XMVECTOR pV = XMVector3TransformCoord(XMLoadFloat3(&record.p), mxObject2World);
			XMVECTOR nV = XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&record.n), mxObject2World));
			XMVECTOR woV = XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&record.wo), mxObject2World));
			XMVECTOR dpduV = XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&record.dpdu), mxObject2World));
			XMVECTOR dpdvV = XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&record.dpdv), mxObject2World));

			SurfaceInteraction result;
			XMStoreFloat3(&result.p, pV);
			XMStoreFloat3(&result.n, nV);
			XMStoreFloat3(&result.wo, woV);
			XMStoreFloat3(&result.dpdu, dpduV);
			XMStoreFloat3(&result.dpdv, dpdvV);
			result.uv = record.uv;
			result.bsdf = record.bsdf;
			result.shape = record.shape;

			*out_tHit = XMVectorGetX(XMVector3Length(pV - XMLoadFloat3(&worldRay.GetOrigin())));
			*out_isect = result;
			return true;
		}
	}
	return false;
}

bool Box::IntersectP(Ray worldRay, float* out_t0, float* out_t1)
{
	XMVECTOR vMax = XMLoadFloat3(&m_aabb.GetVecMax());
	XMVECTOR vMin = XMLoadFloat3(&m_aabb.GetVecMin());

	XMMATRIX mxObject2World = XMLoadFloat4x4(&GetObject2World());
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

	*out_t0 = tNear;
	*out_t1 = tFar;
	return tNear > 0 && tNear < tFar;
}

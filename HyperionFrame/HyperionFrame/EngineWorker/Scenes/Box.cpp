#include "Box.h"
#include "PipelineManager.h"
#include "Interaction.h"

Box::Box()
{
}

Box::Box(const shared_ptr<DXResource>& dxResources) :
	HShape(dxResources)
{
	m_type = ePrimitiveType::BOX;
	m_renderType = eRenderType::Shape;
}

Box::~Box()
{
}

void Box::InitParameters()
{
	HFloat x = 0.5f, y = 0.5f, z = 0.5f;
	m_vertices =
	{
		// -X
		{ HFloat3(-x, +y, +z), HFloat3(-1.0f, 0.0f, 0.0f), HFloat2(0.0f, 1.0f) },
		{ HFloat3(-x, +y, -z), HFloat3(-1.0f, 0.0f, 0.0f), HFloat2(1.0f, 1.0f) },
		{ HFloat3(-x, -y, -z), HFloat3(-1.0f, 0.0f, 0.0f), HFloat2(1.0f, 0.0f) },
		{ HFloat3(-x, -y, +z), HFloat3(-1.0f, 0.0f, 0.0f), HFloat2(0.0f, 0.0f) },

		// +X
		{ HFloat3(+x, +y, -z), HFloat3(1.0f, 0.0f, 0.0f), HFloat2(0.0f, 1.0f) },
		{ HFloat3(+x, +y, +z), HFloat3(1.0f, 0.0f, 0.0f), HFloat2(1.0f, 1.0f) },
		{ HFloat3(+x, -y, +z), HFloat3(1.0f, 0.0f, 0.0f), HFloat2(1.0f, 0.0f) },
		{ HFloat3(+x, -y, -z), HFloat3(1.0f, 0.0f, 0.0f), HFloat2(0.0f, 0.0f) },

		// -Y
		{ HFloat3(-x, -y, -z), HFloat3(0.0f, -1.0f, 0.0f), HFloat2(0.0f, 1.0f) },
		{ HFloat3(+x, -y, -z), HFloat3(0.0f, -1.0f, 0.0f), HFloat2(1.0f, 1.0f) },
		{ HFloat3(+x, -y, +z), HFloat3(0.0f, -1.0f, 0.0f), HFloat2(1.0f, 0.0f) },
		{ HFloat3(-x, -y, +z), HFloat3(0.0f, -1.0f, 0.0f), HFloat2(0.0f, 0.0f) },

		// +Y
		{ HFloat3(-x, +y, +z), HFloat3(0.0f, 1.0f, 0.0f), HFloat2(0.0f, 1.0f) },
		{ HFloat3(+x, +y, +z), HFloat3(0.0f, 1.0f, 0.0f), HFloat2(1.0f, 1.0f) },
		{ HFloat3(+x, +y, -z), HFloat3(0.0f, 1.0f, 0.0f), HFloat2(1.0f, 0.0f) },
		{ HFloat3(-x, +y, -z), HFloat3(0.0f, 1.0f, 0.0f), HFloat2(0.0f, 0.0f) },

		// -Z
		{ HFloat3(-x, +y, -z), HFloat3(0.0f, 0.0f, -1.0f), HFloat2(0.0f, 1.0f) },
		{ HFloat3(+x, +y, -z), HFloat3(0.0f, 0.0f, -1.0f), HFloat2(1.0f, 1.0f) },
		{ HFloat3(+x, -y, -z), HFloat3(0.0f, 0.0f, -1.0f), HFloat2(1.0f, 0.0f) },
		{ HFloat3(-x, -y, -z), HFloat3(0.0f, 0.0f, -1.0f), HFloat2(0.0f, 0.0f) },

		// +Z
		{ HFloat3(+x, +y, +z), HFloat3(0.0f, 0.0f, 1.0f), HFloat2(0.0f, 1.0f) },
		{ HFloat3(-x, +y, +z), HFloat3(0.0f, 0.0f, 1.0f), HFloat2(1.0f, 1.0f) },
		{ HFloat3(-x, -y, +z), HFloat3(0.0f, 0.0f, 1.0f), HFloat2(1.0f, 0.0f) },
		{ HFloat3(+x, -y, +z), HFloat3(0.0f, 0.0f, 1.0f), HFloat2(0.0f, 0.0f) },
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

	m_aabb = AABB(HFloat3(-x, -y, -z), HFloat3(x, y, z));
}

void Box::Update()
{
	// 准备将更新的模型矩阵传递到着色器。
	PipelineManager::s_constantBufferData.model = worldMatrix;

	memcpy(m_mappedConstantBuffer, &PipelineManager::s_constantBufferData, sizeof(PipelineManager::s_constantBufferData));
	memcpy(m_mappedConstantBuffer + 256, &m_cbMeshData, sizeof(m_cbMeshData));
}

void Box::Render(ComPtr<ID3D12GraphicsCommandList> pCommandList)
{
	pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	pCommandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
	pCommandList->IASetIndexBuffer(&m_indexBufferView);
	pCommandList->DrawIndexedInstanced(36, 1, 0, 0, 0);
}

bool Box::Intersect(Ray worldRay, SurfaceInteraction* out_isect, EFloat* out_tHit)
{
	HFloat3 vRayOrig = worldRay.origin.TransformCoord(worldMatrixInv);
	HFloat3 vRayDir = worldRay.direction.TransformNormal(worldMatrixInv).Normalize();
	HFloat3 vRayDirInv = vRayDir.Reciprocal();

	HFloat3 tMax = (m_aabb.max - vRayOrig) * vRayDirInv;
	HFloat3 tMin = (m_aabb.min - vRayOrig) * vRayDirInv;

	HFloat3 t1 = tMin.MinVector(tMax);
	HFloat3 t2 = tMin.MaxVector(tMax);

	HFloat tNear = max(t1.x, max(t1.y, t1.z));
	HFloat tFar = min(t2.x, min(t2.y, t2.z));
	
	SurfaceInteraction record;
	*out_tHit = FLT_MAX;
	if (tNear < tFar && tNear > H_EPSILON)
	{
		for (HUInt i = 0; i < GetFaceCount(); i++)
		{
			Triangle tri = GetFace(i);

			HFloat3 p0 = tri.p[0];
			HFloat3 p1 = tri.p[1];
			HFloat3 p2 = tri.p[2];
			
			HFloat3 p0t = p0 - vRayOrig;
			HFloat3 p1t = p1 - vRayOrig;
			HFloat3 p2t = p2 - vRayOrig;

			HInt kz = 0;
			HFloat kValue = fabsf(vRayDir[0]);
			for (HInt i = 1; i < 4; i++)
			{
				HFloat getByIdx = fabsf(vRayDir[i]);
				if (kValue < getByIdx)
				{
					kValue = getByIdx;
					kz = i;
				}
			}
			HInt kx = kz + 1;
			if (kx == 3) kx = 0;
			HInt ky = kx + 1;
			if (ky == 3) ky = 0;

			HFloat3 d = vRayDir.Permute(kx, ky, kz);
			HFloat3 fp0t = p0t.Permute(kx, ky, kz);
			HFloat3 fp1t = p1t.Permute(kx, ky, kz);
			HFloat3 fp2t = p2t.Permute(kx, ky, kz);

			HFloat Sx = -d.x / d.z;
			HFloat Sy = -d.y / d.z;
			HFloat Sz = 1.f / d.z;
			fp0t.x += Sx * fp0t.z;
			fp0t.y += Sy * fp0t.z;
			fp1t.x += Sx * fp1t.z;
			fp1t.y += Sy * fp1t.z;
			fp2t.x += Sx * fp2t.z;
			fp2t.y += Sy * fp2t.z;

			HFloat e0 = fp1t.x * fp2t.y - fp1t.y * fp2t.x;
			HFloat e1 = fp2t.x * fp0t.y - fp2t.y * fp0t.x;
			HFloat e2 = fp0t.x * fp1t.y - fp0t.y * fp1t.x;

			if (e0 == 0.0f || e1 == 0.0f || e2 == 0.0f) 
			{
				HDouble p2txp1ty = (HDouble)fp2t.x * (HDouble)fp1t.y;
				HDouble p2typ1tx = (HDouble)fp2t.y * (HDouble)fp1t.x;
				e0 = (HFloat)(p2typ1tx - p2txp1ty);
				HDouble p0txp2ty = (HDouble)fp0t.x * (HDouble)fp2t.y;
				HDouble p0typ2tx = (HDouble)fp0t.y * (HDouble)fp2t.x;
				e1 = (HFloat)(p0typ2tx - p0txp2ty);
				HDouble p1txp0ty = (HDouble)fp1t.x * (HDouble)fp0t.y;
				HDouble p1typ0tx = (HDouble)fp1t.y * (HDouble)fp0t.x;
				e2 = (HFloat)(p1typ0tx - p1txp0ty);
			}

			if ((e0 < 0 || e1 < 0 || e2 < 0) && (e0 > 0 || e1 > 0 || e2 > 0))
				continue;
			HFloat det = e0 + e1 + e2;
			if (det == 0) continue;

			fp0t.z *= Sz;
			fp1t.z *= Sz;
			fp2t.z *= Sz;
			HFloat tScaled = e0 * fp0t.z + e1 * fp1t.z + e2 * fp2t.z;
			if (det < 0 && (tScaled >= 0/* || tScaled < ray.tMax * det*/))
				continue;
			else if (det > 0 && (tScaled <= 0/* || tScaled > ray.tMax * det*/))
				continue;

			HFloat invDet = 1 / det;
			HFloat b0 = e0 * invDet;
			HFloat b1 = e1 * invDet;
			HFloat b2 = e2 * invDet;
			HFloat t = tScaled * invDet;

			TriangleUV uv = GetUVs(i);

			HFloat du02 = uv.p[0].x - uv.p[2].x;
			HFloat du12 = uv.p[1].x - uv.p[2].x;
			HFloat dv02 = uv.p[0].y - uv.p[2].y;
			HFloat dv12 = uv.p[1].y - uv.p[2].y;
			HFloat3 dp02 = p0 - p2, dp12 = p1 - p2;
			HFloat detUV = du02 * dv12 - dv02 * du12;
			HFloat invdetUV = 1.0f / det;

			HFloat3 dpdu = dv12 * dp02 - dv02 * dp12;
			HFloat3 dpdv = du02 * dp12 - du12 * dp02;

			HFloat3 pHit = b0 * p0 + b1 * p1 + b2 * p2;
			HFloat2 uvHit = b0 * uv.p[0] + b1 * uv.p[1] + b2 * uv.p[2];

			if (out_tHit->v > t && t > 1e-5f)
			{
				*out_tHit = t;
				HFloat3 hitPos = vRayOrig + t * vRayDir;
				HFloat3 wo = -vRayDir;
				record = SurfaceInteraction(hitPos, uvHit, wo, dpdu, dpdv, this);
			}
		}

		if (record.shape == this)
		{
			// isect 转换成世界坐标
			SurfaceInteraction result;
			result.p = record.p.TransformCoord(worldMatrix);
			result.n = record.n.TransformNormal(worldMatrix).Normalize();
			result.wo = record.wo.TransformNormal(worldMatrix).Normalize();
			result.dpdu = record.dpdu.TransformNormal(worldMatrix).Normalize();
			result.dpdv = record.dpdv.TransformNormal(worldMatrix).Normalize();
			result.uv = record.uv;
			result.bsdf = record.bsdf;
			result.shape = record.shape;

			*out_tHit = (result.p - worldRay.origin).Length();
			*out_isect = result;
			return true;
		}
	}
	return false;
}

bool Box::IntersectP(Ray worldRay, EFloat* out_t0, EFloat* out_t1)
{
	HFloat3 vRayOrig = worldRay.origin.TransformCoord(worldMatrixInv);
	HFloat3 vRayDir = worldRay.direction.TransformNormal(worldMatrixInv).Normalize();
	HFloat3 vRayDirInv = vRayDir.Reciprocal();

	HFloat3 tMax = (m_aabb.max - vRayOrig) * vRayDirInv;
	HFloat3 tMin = (m_aabb.min - vRayOrig) * vRayDirInv;

	HFloat3 t1 = tMin.MinVector(tMax);
	HFloat3 t2 = tMin.MaxVector(tMax);

	HFloat tNear = max(t1.x, max(t1.y, t1.z));
	HFloat tFar = min(t2.x, min(t2.y, t2.z));

	*out_t0 = tNear;
	*out_t1 = tFar;
	return tNear > 0 && tNear < tFar;
}

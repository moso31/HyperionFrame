#include "Sphere.h"
#include "PipelineManager.h"
#include "Interaction.h"

Sphere::Sphere()
{
}

Sphere::Sphere(const shared_ptr<DXResource>& dxResources) :
	HShape(dxResources)
{
	m_type = ePrimitiveType::SPHERE;
	m_renderType = eRenderType::Shape;
}

Sphere::~Sphere()
{
}

void Sphere::InitParameters(HFloat radius, HInt segmentHorizontal, HInt segmentVertical)
{
	HInt currVertIdx = 0;
	for (HInt i = 0; i < segmentVertical; i++)
	{
		HFloat yDown = ((HFloat)i / (HFloat)segmentVertical * 2.0f - 1.0f) * radius;
		HFloat yUp = ((HFloat)(i + 1) / (HFloat)segmentVertical * 2.0f - 1.0f) * radius;
		HFloat radiusDown = sqrtf(radius * radius - yDown * yDown);
		HFloat radiusUp = sqrtf(radius * radius - yUp * yUp);

		for (HInt j = 0; j < segmentHorizontal; j++)
		{
			HFloat segNow = (HFloat)j / (HFloat)segmentHorizontal;
			HFloat segNext = (HFloat)(j + 1) / (HFloat)segmentHorizontal;
			HFloat angleNow = segNow * H_2PI;
			HFloat angleNext = segNext * H_2PI;
			HFloat xNow = sinf(angleNow);
			HFloat zNow = cosf(angleNow);
			HFloat xNext = sinf(angleNext);
			HFloat zNext = cosf(angleNext);

			HFloat3 pNowUp = { xNow * radiusUp, yUp, zNow * radiusUp };
			HFloat3 pNextUp = { xNext * radiusUp, yUp, zNext * radiusUp };
			HFloat3 pNowDown = { xNow * radiusDown, yDown, zNow * radiusDown };
			HFloat3 pNextDown = { xNext * radiusDown, yDown, zNext * radiusDown };

			HFloat2 uvNowUp = { segNow, yUp };
			HFloat2 uvNextUp = { segNext, yUp };
			HFloat2 uvNowDown = { segNow, yDown };
			HFloat2 uvNextDown = { segNext, yDown };

			HFloat invRadius = 1.0f / radius;
			HFloat3 nNowUp, nNowDown, nNextUp, nNextDown;
			nNowUp = pNowUp * invRadius;
			nNextUp = pNextUp * invRadius;
			nNowDown = pNowDown * invRadius;
			nNextDown = pNextDown * invRadius;

			m_vertices.push_back({ pNowUp,		nNowUp,		uvNowUp });
			m_vertices.push_back({ pNextUp,		nNextUp,	uvNextUp });
			m_vertices.push_back({ pNextDown,	nNextDown,	uvNextDown });
			m_vertices.push_back({ pNowDown,	nNowDown,	uvNowDown });

			m_indices.push_back(currVertIdx);
			m_indices.push_back(currVertIdx + 2);
			m_indices.push_back(currVertIdx + 1);
			m_indices.push_back(currVertIdx);
			m_indices.push_back(currVertIdx + 3);
			m_indices.push_back(currVertIdx + 2);

			currVertIdx += 4;
		}
	}

	for (HInt i = 0; i < m_vertices.size(); i++)
	{
		m_aabb.Merge(m_vertices[i].pos);
	}

	m_radius = radius;
	m_segmentVertical = segmentVertical;
	m_segmentHorizontal = segmentHorizontal;
}

void Sphere::Update()
{
	// 准备将更新的模型矩阵传递到着色器。
	PipelineManager::s_constantBufferData.model = worldMatrix;

	memcpy(m_mappedConstantBuffer, &PipelineManager::s_constantBufferData, sizeof(PipelineManager::s_constantBufferData));
	memcpy(m_mappedConstantBuffer + 256, &m_cbMeshData, sizeof(m_cbMeshData));
}

void Sphere::Render(ComPtr<ID3D12GraphicsCommandList> pCommandList)
{
	pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	pCommandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
	pCommandList->IASetIndexBuffer(&m_indexBufferView);
	pCommandList->DrawIndexedInstanced((HUInt)m_indices.size(), 1, 0, 0, 0);
}

bool Sphere::Intersect(Ray worldRay, SurfaceInteraction* out_isect, EFloat* out_tHit)
{
	HFloat3 oErr, dErr;
	Ray ray = worldRay.Transform(worldMatrixInv, oErr, dErr);

	EFloat ox(ray.origin.x, oErr.x), oy(ray.origin.y, oErr.y), oz(ray.origin.z, oErr.z);
	EFloat dx(ray.direction.x, dErr.x), dy(ray.direction.y, dErr.y), dz(ray.direction.z, dErr.z);
	EFloat a = dx * dx + dy * dy + dz * dz;
	EFloat b = 2.0f * (dx * ox + dy * oy + dz * oz);
	EFloat c = ox * ox + oy * oy + oz * oz - EFloat(m_radius) * EFloat(m_radius);

	EFloat t0, t1;
	if (!Quadratic(a, b, c, &t0, &t1)) return false;

	//if (t0.high > ray.tMax) return false;
	if (t1.low < 0) return false;

	EFloat tShapeHit;
	tShapeHit = t0;
	if (tShapeHit.low <= 0.0f)
	{
		tShapeHit = t1;
		//if (tShapeHit.high > ray.tMax) return false;
	}
	*out_tHit = tShapeHit;

	HFloat3 pHit = ray.GetT(tShapeHit.v);
	pHit *= m_radius / pHit.Length();
	if (pHit.x == 0 && pHit.y == 0) pHit.x = 1e-5f * m_radius;
	
	HFloat phi = atan2f(pHit.y, pHit.x);
	if (phi < 0.0f) phi += H_2PI;

	HFloat u = phi / H_2PI;
	HFloat theta = acosf(Clamp(pHit.z / m_radius, -1.0f, 1.0f));
	HFloat v = theta / H_PI;

	HFloat zRadius = sqrtf(pHit.x * pHit.x + pHit.y * pHit.y);
	HFloat invZRadius = 1.0f / zRadius;
	HFloat cosPhi = pHit.x * invZRadius;
	HFloat sinPhi = pHit.y * invZRadius;
	HFloat3 dpdu(-H_2PI * pHit.y, H_2PI * pHit.x, 0.0f);
	HFloat3 dpdv = HFloat3(H_PI * pHit.z * cosPhi, H_PI * pHit.z * sinPhi, H_PI * -m_radius * sinf(theta));

	HFloat3 pErr = gamma(5) * pHit;

	SurfaceInteraction isect;
	isect.p = pHit.TransformCoord(worldMatrix, pErr, isect.pError);
	isect.wo = -ray.direction.TransformNormal(worldMatrix).Normalize();
	isect.dpdu = dpdu.TransformNormal(worldMatrix).Normalize();
	isect.dpdv = dpdv.TransformNormal(worldMatrix).Normalize();
	isect.n = dpdv.Cross(dpdu).Normalize();
	isect.uv = HFloat2(u, v);
	isect.shape = this;

	*out_isect = isect;

	return true;
}

bool Sphere::IntersectP(Ray worldRay, EFloat* out_t0, EFloat* out_t1)
{
	HFloat3 oErr, dErr;
	Ray ray = worldRay.Transform(worldMatrixInv, oErr, dErr);

	EFloat ox(ray.origin.x, oErr.x), oy(ray.origin.y, oErr.y), oz(ray.origin.z, oErr.z);
	EFloat dx(ray.direction.x, dErr.x), dy(ray.direction.y, dErr.y), dz(ray.direction.z, dErr.z);
	EFloat a = dx * dx + dy * dy + dz * dz;
	EFloat b = 2.0f * (dx * ox + dy * oy + dz * oz);
	EFloat c = ox * ox + oy * oy + oz * oz - EFloat(m_radius) * EFloat(m_radius);

	if (!Quadratic(a, b, c, out_t0, out_t1)) return false;

	return out_t1->low > 0.0f;
}

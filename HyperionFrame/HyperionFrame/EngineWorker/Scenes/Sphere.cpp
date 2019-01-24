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

void Sphere::InitParameters(float radius, int segmentHorizontal, int segmentVertical)
{
	int currVertIdx = 0;
	for (int i = 0; i < segmentVertical; i++)
	{
		float yDown = ((float)i / (float)segmentVertical * 2.0f - 1.0f) * radius;
		float yUp = ((float)(i + 1) / (float)segmentVertical * 2.0f - 1.0f) * radius;
		float radiusDown = sqrtf(radius * radius - yDown * yDown);
		float radiusUp = sqrtf(radius * radius - yUp * yUp);

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

			float invRadius = 1.0f / radius;
			XMFLOAT3 nNowUp, nNowDown, nNextUp, nNextDown;
			XMStoreFloat3(&nNowUp, XMLoadFloat3(&pNowUp) * invRadius);
			XMStoreFloat3(&nNextUp, XMLoadFloat3(&pNextUp) * invRadius);
			XMStoreFloat3(&nNowDown, XMLoadFloat3(&pNowDown) * invRadius);
			XMStoreFloat3(&nNextDown, XMLoadFloat3(&pNextDown) * invRadius);

			m_vertices.push_back({ pNowUp,		nNowUp,		uvNowUp });
			m_vertices.push_back({ pNextUp,		nNextUp,	uvNextUp });
			m_vertices.push_back({ pNextDown,	nNextDown,	uvNextDown });
			m_vertices.push_back({ pNowDown,	nNowDown,	uvNowDown });

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

void Sphere::Update()
{
	// 准备将更新的模型矩阵传递到着色器。
	XMStoreFloat4x4(&PipelineManager::s_constantBufferData.model, XMMatrixTranspose(XMLoadFloat4x4(&GetObject2World())));

	memcpy(m_mappedConstantBuffer, &PipelineManager::s_constantBufferData, sizeof(PipelineManager::s_constantBufferData));
	memcpy(m_mappedConstantBuffer + 256, &m_cbMeshData, sizeof(m_cbMeshData));
}

void Sphere::Render(ComPtr<ID3D12GraphicsCommandList> pCommandList)
{
	pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	pCommandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
	pCommandList->IASetIndexBuffer(&m_indexBufferView);
	pCommandList->DrawIndexedInstanced((UINT)m_indices.size(), 1, 0, 0, 0);
}

bool Sphere::Intersect(Ray worldRay, SurfaceInteraction* out_isect, float* out_tHit)
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
		return false;

	if (t1 < H_EPSILON)
		return false;
	*out_tHit = t0;
	if (*out_tHit <= 0.0f)
		*out_tHit = t1;

	XMFLOAT3 pHit = ray.GetT(*out_tHit);
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
	return true;
}

bool Sphere::IntersectP(Ray worldRay, float* out_t0, float* out_t1)
{
	XMFLOAT3 ro, rd;
	XMStoreFloat3(&ro, XMVector3TransformCoord(XMLoadFloat3(&worldRay.GetOrigin()), XMLoadFloat4x4(&GetWorld2Object())));
	XMStoreFloat3(&rd, XMVector3TransformNormal(XMLoadFloat3(&worldRay.GetDirection()), XMLoadFloat4x4(&GetWorld2Object())));
	Ray ray(ro, rd);

	float a = rd.x * rd.x + rd.y * rd.y + rd.z * rd.z;
	float b = 2 * (rd.x * ro.x + rd.y * ro.y + rd.z * ro.z);
	float c = ro.x * ro.x + ro.y * ro.y + ro.z * ro.z - m_radius * m_radius;

	if (!Quadratic(a, b, c, *out_t0, *out_t1)) return false;

	return *out_t1 > H_EPSILON;
}

#include "Shape.h"

Shape::Shape()
{
}

Shape::Shape(const shared_ptr<DXResource>& dxResources, Camera * camera) :
	m_dxResources(dxResources),
	m_camera(camera)
{
}

Shape::~Shape()
{
}

AABB Shape::GetAABB()
{
	return m_aabb;
}

AABB Shape::GenerateAABB()
{
	AABB result;
	for (size_t i = 0; i < m_vertices.size(); i++)
		result.Merge(m_vertices[i].pos);
	return result;
}

UINT Shape::GetFaceCount()
{
	return m_indices.size() / 3;
}

HTriangle Shape::GetFace(UINT faceIndex)
{
	HTriangle result;
	for (int i = 0; i < 3; i++)
		result.p[i] = m_vertices[m_indices[faceIndex * 3 + i]].pos;
	return result;
}

void Shape::Intersect(Ray worldRay, XMFLOAT3 & out_hitPos, int & out_hitIndex)
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
			}
		}
	}
}

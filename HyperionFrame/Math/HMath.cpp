#include "HMath.h"

namespace HIFrameMath
{
	Ray::Ray(const XMFLOAT3 _orig, const XMFLOAT3 _dir)
	{
		origin = _orig;
		direction = _dir;
	}

	XMFLOAT3 Ray::GetOrigin()
	{
		return origin;
	}

	XMFLOAT3 Ray::GetDirection()
	{
		return direction;
	}

	AABB::AABB(XMFLOAT3 _min, XMFLOAT3 _max)
	{
		min = _min;
		max = _max;
	}

	XMFLOAT3 AABB::GetCenter()
	{
		XMFLOAT3 result;
		XMStoreFloat3(&result, (XMLoadFloat3(&min) + XMLoadFloat3(&max)) * 0.5f);
		return result;
	}

	XMFLOAT3 AABB::GetExtent()
	{
		XMFLOAT3 result;
		XMStoreFloat3(&result, (XMLoadFloat3(&max) - XMLoadFloat3(&min)));
		return result;
	}

	XMFLOAT3 AABB::GetVecMin()
	{
		return min;
	}

	XMFLOAT3 AABB::GetVecMax()
	{
		return max;
	}

	void AABB::Merge(XMFLOAT3 point)
	{
		XMVECTOR vMax = XMVectorMax(XMLoadFloat3(&max), XMLoadFloat3(&point));
		XMVECTOR vMin = XMVectorMin(XMLoadFloat3(&min), XMLoadFloat3(&point));
		XMStoreFloat3(&max, vMax);
		XMStoreFloat3(&min, vMin);
	}

	bool RayIntersectP(Ray ray, AABB aabb)
	{
		XMFLOAT3 vTemp;
		vTemp = aabb.GetVecMax();
		XMVECTOR vMax = XMLoadFloat3(&vTemp);
		vTemp = aabb.GetVecMin();
		XMVECTOR vMin = XMLoadFloat3(&vTemp);

		XMVECTOR vRayOrig = XMLoadFloat3(&ray.GetOrigin());
		XMVECTOR vRayDirReciprocal = XMVectorReciprocal(XMLoadFloat3(&ray.GetDirection()));

		XMVECTOR tMax = (vMax - vRayOrig) * vRayDirReciprocal;
		XMVECTOR tMin = (vMin - vRayOrig) * vRayDirReciprocal;

		XMFLOAT3 t1, t2;
		XMStoreFloat3(&t1, XMVectorMin(tMin, tMax));
		XMStoreFloat3(&t2, XMVectorMax(tMin, tMax));

		float tNear = max(t1.x, max(t1.y, t1.z));
		float tFar = min(t2.x, min(t2.y, t2.z));

		return tNear < tFar;
	}
}

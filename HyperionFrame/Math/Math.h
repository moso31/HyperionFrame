#pragma once
#include "Shape.h"

namespace HIFrameMath
{
	class Ray
	{
	public:
		Ray(const XMFLOAT3 _orig = { 0.0f, 0.0f, 0.0f }, const XMFLOAT3 _dir = { 0.0f, 0.0f, 0.0f })
		{
			origin = _orig;
			direction = _dir;
		}

		~Ray() {}

		XMFLOAT3 GetOrigin()
		{
			return origin;
		}

		XMFLOAT3 GetDirection()
		{
			return direction;
		}

	private:
		XMFLOAT3 origin;
		XMFLOAT3 direction;
	};

	class AABB
	{
	public:
		AABB(XMFLOAT3 _min = { 0.0f, 0.0f, 0.0f }, XMFLOAT3 _max = { 0.0f, 0.0f, 0.0f })
		{
			min = _min;
			max = _max;
		}

		~AABB() {}

		XMFLOAT3 GetCenter()
		{
			XMFLOAT3 result;
			XMStoreFloat3(&result, (XMLoadFloat3(&min) + XMLoadFloat3(&max)) * 0.5f);
			return result;
		}

		XMFLOAT3 GetExtent()
		{
			XMFLOAT3 result;
			XMStoreFloat3(&result, (XMLoadFloat3(&max) - XMLoadFloat3(&min)));
			return result;
		}

		XMFLOAT3 GetVecMin()
		{
			return min;
		}

		XMFLOAT3 GetVecMax()
		{
			return max;
		}

	private:
		XMFLOAT3 min;
		XMFLOAT3 max;
	};

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
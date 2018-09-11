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

	bool RayIntersectP(const Ray Ray, const AABB aabb)
	{
		
	}
}
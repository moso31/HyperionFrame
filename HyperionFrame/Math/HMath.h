#pragma once
#include "header.h"

#define max(a,b)            (((a) > (b)) ? (a) : (b))
#define min(a,b)            (((a) < (b)) ? (a) : (b))

namespace HIFrameMath
{
	class Ray
	{
	public:
		Ray(const XMFLOAT3 _orig = { 0.0f, 0.0f, 0.0f }, const XMFLOAT3 _dir = { 0.0f, 0.0f, 0.0f });
		~Ray() {}

		XMFLOAT3 GetOrigin();
		XMFLOAT3 GetDirection();

	private:
		XMFLOAT3 origin;
		XMFLOAT3 direction;
	};

	class AABB
	{
	public:
		AABB(XMFLOAT3 _min = { 0.0f, 0.0f, 0.0f }, XMFLOAT3 _max = { 0.0f, 0.0f, 0.0f });
		~AABB() {}

		XMFLOAT3 GetCenter();
		XMFLOAT3 GetExtent();
		XMFLOAT3 GetVecMin();
		XMFLOAT3 GetVecMax();

		void Merge(XMFLOAT3 point);

	private:
		XMFLOAT3 min;
		XMFLOAT3 max;
	};

	bool RayIntersectP(Ray ray, AABB aabb);
}
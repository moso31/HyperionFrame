#pragma once
#include "header.h"

template <typename T, typename U, typename V>
inline T Clamp(T val, U low, V high) {
	if ((T)val < (T)low) return (T)low;
	else if ((T)val > (T)high) return (T)high;
	else return (T)val;
}

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

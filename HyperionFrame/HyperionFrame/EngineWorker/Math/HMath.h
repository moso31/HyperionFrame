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
	Ray(const XMFLOAT3& _orig = { 0.0f, 0.0f, 0.0f }, const XMFLOAT3& _dir = { 0.0f, 0.0f, 0.0f });
	~Ray() {}

	XMFLOAT3 GetOrigin();
	XMFLOAT3 GetDirection();
	XMFLOAT3 GetT(float t);

private:
	XMFLOAT3 origin;
	XMFLOAT3 direction;
};

class Segment
{
public:
	Segment(const XMFLOAT3& point1, const XMFLOAT3& point2);
	~Segment() {}

	XMFLOAT3 point1;
	XMFLOAT3 point2;
};

class AABB
{
public:
	AABB();
	AABB(XMFLOAT3 p);
	AABB(XMFLOAT3 _min, XMFLOAT3 _max);
	~AABB() {}

	XMFLOAT3 GetCenter() const;
	XMFLOAT3 GetExtent() const;
	XMFLOAT3 GetVecMin() const;
	XMFLOAT3 GetVecMax() const;

	// 获取指定点在当前AABB中的相对位置。
	// 点最小角时将表示为(0, 0, 0)，最大角是(1, 1, 1)
	XMFLOAT3 Offset(XMFLOAT3& p) const;

	void Merge(AABB aabb);
	void Merge(XMFLOAT3 point);

	bool IntersectP(Ray ray, float* hit0, float* hit1);

	// 获取当前包围盒的表面积
	float GetSurfaceArea();

	int GetMaximumExtent();

private:
	XMFLOAT3 min;
	XMFLOAT3 max;
};

bool Quadratic(float a, float b, float c, float& out_t0, float& out_t1);

bool RayIntersectP(Ray ray, AABB aabb);

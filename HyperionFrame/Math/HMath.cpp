#include "HMath.h"

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

XMFLOAT3 Ray::GetT(float t)
{
	XMVECTOR oV = XMLoadFloat3(&origin);
	XMVECTOR dV = XMLoadFloat3(&direction);
	XMVECTOR pV = oV + t * dV;
	XMFLOAT3 result;
	XMStoreFloat3(&result, pV);
	return result;
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

void AABB::Merge(AABB aabb)
{
	XMVECTOR vMax = XMVectorMax(XMLoadFloat3(&max), XMLoadFloat3(&aabb.max));
	XMVECTOR vMin = XMVectorMin(XMLoadFloat3(&min), XMLoadFloat3(&aabb.min));
	XMStoreFloat3(&max, vMax);
	XMStoreFloat3(&min, vMin);
}

void AABB::Merge(XMFLOAT3 point)
{
	XMVECTOR vMax = XMVectorMax(XMLoadFloat3(&max), XMLoadFloat3(&point));
	XMVECTOR vMin = XMVectorMin(XMLoadFloat3(&min), XMLoadFloat3(&point));
	XMStoreFloat3(&max, vMax);
	XMStoreFloat3(&min, vMin);
}

bool AABB::IntersectP(Ray ray, float* hit0, float* hit1)
{
	XMVECTOR vMax = XMLoadFloat3(&max);
	XMVECTOR vMin = XMLoadFloat3(&min);

	XMVECTOR vRayOrig = XMLoadFloat3(&ray.GetOrigin());
	XMVECTOR vRayDir = XMLoadFloat3(&ray.GetDirection());
	XMVECTOR vRayDirReciprocal = XMVectorReciprocal(vRayDir);

	XMVECTOR tMax = (vMax - vRayOrig) * vRayDirReciprocal;
	XMVECTOR tMin = (vMin - vRayOrig) * vRayDirReciprocal;

	XMFLOAT3 t1, t2;
	XMStoreFloat3(&t1, XMVectorMin(tMin, tMax));
	XMStoreFloat3(&t2, XMVectorMax(tMin, tMax));

	float tNear = std::max(t1.x, std::max(t1.y, t1.z));
	float tFar = std::min(t2.x, std::min(t2.y, t2.z));

	*hit0 = tNear;
	*hit1 = tFar;
	return tNear > 0 && tNear < tFar;
}

int AABB::GetMaximumExtent()
{
	XMFLOAT3 dim = GetExtent();
	if (dim.x > dim.y)
	{
		return dim.x > dim.z ? 0 : 2;
	}
	else
	{
		return dim.y > dim.z ? 1 : 2;
	}
}

bool Quadratic(float a, float b, float c, float& out_t0, float& out_t1)
{
	if (abs(a) < H_EPSILON) a = 0.0f;
	if (abs(b) < H_EPSILON) b = 0.0f;
	if (abs(c) < H_EPSILON) c = 0.0f;

	float dt = (b * b - 4 * a * c);
	if (dt < 0.0f)
		return false;

	float sqrtdt = sqrt(dt);
	float q0 = b + sqrtdt, q1 = b - sqrtdt;

	float onediv2a = -0.5f / a;

	out_t0 = q0 * onediv2a;
	out_t1 = q1 * onediv2a;

	if (out_t0 > out_t1) 
		swap(out_t0, out_t1);
	return true;
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

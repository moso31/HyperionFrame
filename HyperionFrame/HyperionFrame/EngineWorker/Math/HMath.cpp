#include "HMath.h"

Ray::Ray(const XMFLOAT3& _orig, const XMFLOAT3& _dir) :
	origin(_orig),
	direction(_dir)
{
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

Segment::Segment(const XMFLOAT3 & point1, const XMFLOAT3 & point2) :
	point1(point1),
	point2(point2)
{
}

AABB::AABB() :
	min({ FLT_MAX, FLT_MAX, FLT_MAX }),
	max({ -FLT_MAX, -FLT_MAX, -FLT_MAX })
{
}

AABB::AABB(XMFLOAT3 p) :
	min(p),
	max(p)
{
}

AABB::AABB(XMFLOAT3 _min, XMFLOAT3 _max) :
	min(_min),
	max(_max)
{
}

XMFLOAT3 AABB::GetCenter() const
{
	XMFLOAT3 result;
	XMStoreFloat3(&result, (XMLoadFloat3(&min) + XMLoadFloat3(&max)) * 0.5f);
	return result;
}

XMFLOAT3 AABB::GetExtent() const
{
	XMFLOAT3 result;
	XMStoreFloat3(&result, (XMLoadFloat3(&max) - XMLoadFloat3(&min)));
	return result;
}

XMFLOAT3 AABB::GetVecMin() const
{
	return min;
}

XMFLOAT3 AABB::GetVecMax() const
{
	return max;
}

XMFLOAT3 AABB::Offset(XMFLOAT3 & p) const
{
	XMVECTOR resultV = (XMLoadFloat3(&p) - XMLoadFloat3(&min)) / (XMLoadFloat3(&max) - XMLoadFloat3(&min));
	XMFLOAT3 result;
	XMStoreFloat3(&result, resultV);
	return result;
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
	*hit1 = tFar * (1 + 2 * gamma(3));
	return tFar > 0 && tNear < tFar;
}

float AABB::GetSurfaceArea() 
{
	XMFLOAT3 v = GetExtent();
	XMLoadFloat3(&max) - XMLoadFloat3(&min);
	return (v.x * v.y + v.y * v.z + v.x * v.z) * 2.0f;
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

	float inv2a = -0.5f / a;

	out_t0 = q0 * inv2a;
	out_t1 = q1 * inv2a;

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
	
	tFar *= (1 + 2 * gamma(3));
	return tNear < tFar;
}

EFloat::EFloat(float v, float err) :
	v(v)
{
	if (err == 0.) low = high = v;
	else
	{
		low = NextFloatDown(v - err);
		high = NextFloatUp(v + err);
	}
	ld = v;
	Check();
}

EFloat::EFloat(float v, long double ld, float err) : 
	EFloat(v, err)
{
	this->ld = ld;
	Check();
}

EFloat::EFloat(const EFloat & ef)
{
	ef.Check();
	v = ef.v;
	ld = ef.ld;
	low = ef.low;
	high = ef.high;
}

EFloat EFloat::operator+(EFloat other) const
{
	EFloat result;
	result.v = v + other.v;
	result.ld = ld + other.ld;

	result.low = NextFloatDown(low + other.low);
	result.high = NextFloatUp(high + other.high);
	result.Check();
	return result;
}

EFloat EFloat::operator-(EFloat other) const
{
	EFloat result;
	result.v = v - other.v;
	result.ld = ld - other.ld;

	result.low = NextFloatDown(low - other.low);
	result.high = NextFloatUp(high - other.high);
	result.Check();
	return result;
}

EFloat EFloat::operator*(EFloat other) const
{
	EFloat result;
	result.v = v * other.v;
	result.ld = ld * other.ld;

	float pro[4] = { low * other.low, low * other.high, high * other.low, high * other.high };
	result.low = NextFloatDown((min(pro[0], pro[1]), min(pro[2], pro[3])));
	result.high = NextFloatUp((max(pro[0], pro[1]), max(pro[2], pro[3])));
	result.Check();
	return result;
}

EFloat EFloat::operator/(EFloat other) const
{
	EFloat result;
	result.v = v / other.v;
	result.ld = ld / other.ld;

	if (other.low < 0 && other.high > 0)
	{
		result.low = -INFINITY;
		result.high = INFINITY;
	}
	else
	{
		float div[4] = { low / other.low, low / other.high, high / other.low, high / other.high };
		result.low = NextFloatDown((min(div[0], div[1]), min(div[2], div[3])));
		result.high = NextFloatUp((max(div[0], div[1]), max(div[2], div[3])));
	}
	result.Check();
	return result;
}

EFloat EFloat::operator-() const
{
	EFloat result;
	result.v = -v;
	result.ld = -ld;
	result.low = -high;
	result.high = -low;
	result.Check();
	return EFloat();
}

bool EFloat::operator==(EFloat other) const
{
	return v == other.v;
}

void EFloat::Check() const
{
	if (!isinf(low) && !isnan(low) && !isinf(high) && !isnan(high))
		assert(low <= high);
	if (!isinf(v) && !isnan(v)) 
	{
		assert(low <= ld);
		assert(ld <= high);
	}
}

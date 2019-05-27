#include "HEngineMath.h"

Ray::Ray(const HFloat3& _orig, const HFloat3& _dir) :
	origin(_orig),
	direction(_dir)
{
}

HFloat3 Ray::GetT(HFloat t)
{
	return origin + t * direction;
}

Segment::Segment(const HFloat3 & point1, const HFloat3 & point2) :
	point1(point1),
	point2(point2)
{
}

AABB::AABB() :
	min({ FLT_MAX, FLT_MAX, FLT_MAX }),
	max({ -FLT_MAX, -FLT_MAX, -FLT_MAX })
{
}

AABB::AABB(HFloat3 p) :
	min(p),
	max(p)
{
}

AABB::AABB(HFloat3 _min, HFloat3 _max) :
	min(_min),
	max(_max)
{
}

HFloat3 AABB::GetCenter() const
{
	return (min + max) * 0.5f;
}

HFloat3 AABB::GetExtent() const
{
	return max - min;
}

HFloat3 AABB::Offset(HFloat3 & p) const
{
	return (p - min) / (max - min);
}

void AABB::Merge(AABB aabb)
{
	max = max.MaxVector(aabb.max);
	min = min.MinVector(aabb.min);
}

void AABB::Merge(HFloat3 point)
{
	max = max.MaxVector(point);
	min = min.MinVector(point);
}

bool AABB::IntersectP(Ray ray, HFloat* hit0, HFloat* hit1)
{
	HFloat3 vRayDirInv = ray.origin.Reciprocal();
	HFloat3 tMax = (max - ray.origin) * vRayDirInv;
	HFloat3 tMin = (min - ray.origin) * vRayDirInv;

	HFloat3 t1 = tMin.MinVector(tMax);
	HFloat3 t2 = tMin.MaxVector(tMax);

	HFloat tNear = std::max(t1.x, std::max(t1.y, t1.z));
	HFloat tFar = std::min(t2.x, std::min(t2.y, t2.z));

	*hit0 = tNear;
	*hit1 = tFar * (1 + 2 * gamma(3));
	return tFar > 0 && tNear < tFar;
}

HFloat AABB::GetSurfaceArea() 
{
	HFloat3 v = GetExtent();
	return (v.x * v.y + v.y * v.z + v.x * v.z) * 2.0f;
}

HInt AABB::GetMaximumExtent()
{
	HFloat3 dim = GetExtent();
	if (dim.x > dim.y)
	{
		return dim.x > dim.z ? 0 : 2;
	}
	else
	{
		return dim.y > dim.z ? 1 : 2;
	}
}

bool Quadratic(HFloat a, HFloat b, HFloat c, HFloat& out_t0, HFloat& out_t1)
{
	if (abs(a) < H_EPSILON) a = 0.0f;
	if (abs(b) < H_EPSILON) b = 0.0f;
	if (abs(c) < H_EPSILON) c = 0.0f;

	HFloat dt = (b * b - 4 * a * c);
	if (dt < 0.0f)
		return false;

	HFloat sqrtdt = sqrtf(dt);
	HFloat q0 = b + sqrtdt, q1 = b - sqrtdt;

	HFloat inv2a = -0.5f / a;

	out_t0 = q0 * inv2a;
	out_t1 = q1 * inv2a;

	if (out_t0 > out_t1) 
		swap(out_t0, out_t1);
	return true;
}

bool RayIntersectP(Ray ray, AABB aabb)
{
	HFloat3 vRayDirInv = ray.origin.Reciprocal();
	HFloat3 tMax = (aabb.max - ray.origin) * vRayDirInv;
	HFloat3 tMin = (aabb.min - ray.origin) * vRayDirInv;

	HFloat3 t1 = tMin.MinVector(tMax);
	HFloat3 t2 = tMin.MaxVector(tMax);

	HFloat tNear = max(t1.x, max(t1.y, t1.z));
	HFloat tFar = min(t2.x, min(t2.y, t2.z));
	
	tFar *= (1 + 2 * gamma(3));
	return tNear < tFar;
}

EFloat::EFloat(HFloat v, HFloat err) :
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

EFloat::EFloat(HFloat v, HDouble ld, HFloat err) : 
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

	HFloat pro[4] = { low * other.low, low * other.high, high * other.low, high * other.high };
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
		HFloat div[4] = { low / other.low, low / other.high, high / other.low, high / other.high };
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

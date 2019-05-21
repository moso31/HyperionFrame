#pragma once
#include "header.h"

static const HFloat H_MACHINEEPSLION = numeric_limits<HFloat>::epsilon() * 0.5f;

template <typename T, typename U, typename V>
inline T Clamp(T val, U low, V high) {
	if ((T)val < (T)low) return (T)low;
	else if ((T)val > (T)high) return (T)high;
	else return (T)val;
}

inline HFloat gamma(HInt n)
{
	return (n * H_MACHINEEPSLION) / (1 - n * H_MACHINEEPSLION);
}

inline HUInt FloatToBits(HFloat f) 
{
	HUInt ui;
	memcpy(&ui, &f, sizeof(HFloat));
	return ui;
}

inline HFloat BitsToFloat(HUInt ui)
{
	HFloat f;
	memcpy(&f, &ui, sizeof(HUInt));
	return f;
}

inline HUInt64 FloatToBits(HDouble f) 
{
	HUInt64 ui;
	memcpy(&ui, &f, sizeof(HDouble));
	return ui;
}

inline HDouble BitsToFloat(HUInt64 ui) 
{
	HDouble f;
	memcpy(&f, &ui, sizeof(HUInt64));
	return f;
}

inline HFloat NextFloatUp(HFloat v) {
	if (isinf(v) && v > 0.) return v;
	if (v == -0.f) v = 0.f;
	HUInt ui = FloatToBits(v);
	if (v >= 0) ++ui;
	else --ui;
	return BitsToFloat(ui);
}

inline HFloat NextFloatDown(HFloat v) 
{
	if (isinf(v) && v < 0.) return v;
	if (v == 0.f) v = -0.f;
	HUInt ui = FloatToBits(v);
	if (v > 0) --ui;
	else ++ui;
	return BitsToFloat(ui);
}

inline HDouble NextFloatUp(HDouble v, HInt delta = 1) 
{
	if (isinf(v) && v > 0.) return v;
	if (v == -0.f) v = 0.f;
	HUInt64 ui = FloatToBits(v);
	if (v >= 0.) ui += delta;
	else ui -= delta;
	return BitsToFloat(ui);
}

inline HDouble NextFloatDown(HDouble v, HInt delta = 1) 
{
	if (isinf(v) && v < 0.) return v;
	if (v == 0.f) v = -0.f;
	HUInt64 ui = FloatToBits(v);
	if (v > 0.) ui -= delta;
	else ui += delta;
	return BitsToFloat(ui);
}

class EFloat
{
public:
	EFloat() {}
	EFloat(HFloat v, HFloat err = 0.0f);
	EFloat(HFloat v, HDouble ld, HFloat err);
	EFloat(const EFloat& ef);

	EFloat operator+(EFloat other) const;
	EFloat operator-(EFloat other) const;
	EFloat operator*(EFloat other) const;
	EFloat operator/(EFloat other) const;
	EFloat operator-() const;
	bool operator==(EFloat other) const;
	explicit operator HFloat() const { return v; }
	explicit operator HDouble() const { return ld; }

	void Check() const;
	HFloat AbsoluteError() { return high - low; }
	HFloat RelativeError() { return (HFloat)(abs((ld - v) / ld)); }

public:
	HFloat v;	// value
	HFloat low, high;	// absolute error
	HDouble ld;		// double value, for more precision.

private:
	friend inline EFloat sqrt(EFloat other);
	friend inline EFloat abs(EFloat other);
	friend inline bool Quadratic(EFloat A, EFloat B, EFloat C, EFloat *t0, EFloat *t1);
};

// EFloat Inline Functions
inline EFloat operator*(HFloat f, EFloat other) { return EFloat(f) * other; }

inline EFloat operator/(HFloat f, EFloat other) { return EFloat(f) / other; }

inline EFloat operator+(HFloat f, EFloat other) { return EFloat(f) + other; }

inline EFloat operator-(HFloat f, EFloat other) { return EFloat(f) - other; }

inline EFloat sqrt(EFloat other) 
{
	EFloat r;
	r.v = sqrt(other.v);
	r.ld = sqrt(other.ld);
	r.low = NextFloatDown(sqrt(other.low));
	r.high = NextFloatUp(sqrt(other.high));
	r.Check();
	return r;
}

inline EFloat abs(EFloat other)
{
	if (other.low >= 0)
	{
		// 浮点范围全部>=0，肯定为正，返回原值。
		return other;
	}
	else if (other.high <= 0)
	{
		// 浮点范围全部<=0，肯定为负，全部加符号，返回原值。
		EFloat result;
		result.v = -other.v;
		result.ld = -other.ld;
		result.low = -other.low;
		result.high = -other.high;
		result.Check();
		return result;
	}
	else
	{
		// 浮点范围同时涵盖正负数：
		// 取绝对值后，最小值肯定为0，最大值是正负数中实数值最大的那个。
		EFloat result;
		result.v = abs(other.v);
		result.ld = abs(other.ld);
		result.low = 0;
		result.high = max(-other.low, other.high);
		result.Check();
		return result;
	}
}

inline bool Quadratic(EFloat A, EFloat B, EFloat C, EFloat * t0, EFloat * t1)
{
	HDouble delta = (HDouble)B.v * (HDouble)B.v - 4.0 * (HDouble)A.v * HDouble(C.v);
	if (delta < 0.0) 
		return false;
	HDouble sqrtDelta = sqrt(delta);
	EFloat sqrtDeltaFlt((HFloat)sqrtDelta, H_MACHINEEPSLION * (HFloat)sqrtDelta);

	EFloat q;
	if ((HFloat)B < 0) q = -0.5 * (B - sqrtDeltaFlt);
	else q = -0.5 * (B + sqrtDeltaFlt);
	*t0 = q / A;
	*t1 = C / q;
	if ((HFloat)*t0 > (HFloat)*t1) 
		swap(*t0, *t1);
	return true;
}

class Ray
{
public:
	Ray(const HFloat3& _orig = { 0.0f, 0.0f, 0.0f }, const HFloat3& _dir = { 0.0f, 0.0f, 0.0f });
	~Ray() {}

	HFloat3 GetT(HFloat t);

public:
	HFloat3 origin;
	HFloat3 direction;
};

class Segment
{
public:
	Segment(const HFloat3& point1, const HFloat3& point2);
	~Segment() {}

	HFloat3 point1;
	HFloat3 point2;
};

class AABB
{
public:
	AABB();
	AABB(HFloat3 p);
	AABB(HFloat3 _min, HFloat3 _max);
	~AABB() {}

	HFloat3 GetCenter() const;
	HFloat3 GetExtent() const;

	// 获取指定点在当前AABB中的相对位置。
	// 点最小角时将表示为(0, 0, 0)，最大角是(1, 1, 1)
	HFloat3 Offset(HFloat3& p) const;

	void Merge(AABB aabb);
	void Merge(HFloat3 point);

	bool IntersectP(Ray ray, HFloat* hit0, HFloat* hit1);

	// 获取当前包围盒的表面积
	HFloat GetSurfaceArea();

	HInt GetMaximumExtent();

public:
	HFloat3 min;
	HFloat3 max;
};

bool Quadratic(HFloat a, HFloat b, HFloat c, HFloat& out_t0, HFloat& out_t1);

bool RayIntersectP(Ray ray, AABB aabb);

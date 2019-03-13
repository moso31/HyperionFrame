#pragma once
#include "header.h"

static const float H_MACHINEEPSLION = numeric_limits<float>::epsilon() * 0.5f;

template <typename T, typename U, typename V>
inline T Clamp(T val, U low, V high) {
	if ((T)val < (T)low) return (T)low;
	else if ((T)val > (T)high) return (T)high;
	else return (T)val;
}

inline float gamma(int n)
{
	return (n * H_MACHINEEPSLION) / (1 - n * H_MACHINEEPSLION);
}

inline uint32_t FloatToBits(float f) 
{
	uint32_t ui;
	memcpy(&ui, &f, sizeof(float));
	return ui;
}

inline float BitsToFloat(uint32_t ui)
{
	float f;
	memcpy(&f, &ui, sizeof(uint32_t));
	return f;
}

inline uint64_t FloatToBits(double f) 
{
	uint64_t ui;
	memcpy(&ui, &f, sizeof(double));
	return ui;
}

inline double BitsToFloat(uint64_t ui) 
{
	double f;
	memcpy(&f, &ui, sizeof(uint64_t));
	return f;
}

inline float NextFloatUp(float v) {
	if (isinf(v) && v > 0.) return v;
	if (v == -0.f) v = 0.f;
	uint32_t ui = FloatToBits(v);
	if (v >= 0) ++ui;
	else --ui;
	return BitsToFloat(ui);
}

inline float NextFloatDown(float v) 
{
	if (isinf(v) && v < 0.) return v;
	if (v == 0.f) v = -0.f;
	uint32_t ui = FloatToBits(v);
	if (v > 0) --ui;
	else ++ui;
	return BitsToFloat(ui);
}

inline double NextFloatUp(double v, int delta = 1) 
{
	if (isinf(v) && v > 0.) return v;
	if (v == -0.f) v = 0.f;
	uint64_t ui = FloatToBits(v);
	if (v >= 0.) ui += delta;
	else ui -= delta;
	return BitsToFloat(ui);
}

inline double NextFloatDown(double v, int delta = 1) 
{
	if (isinf(v) && v < 0.) return v;
	if (v == 0.f) v = -0.f;
	uint64_t ui = FloatToBits(v);
	if (v > 0.) ui -= delta;
	else ui += delta;
	return BitsToFloat(ui);
}

class EFloat
{
public:
	EFloat() {}
	EFloat(float v, float err = 0.0f);
	EFloat(float v, long double ld, float err);
	EFloat(const EFloat& ef);

	EFloat operator+(EFloat other) const;
	EFloat operator-(EFloat other) const;
	EFloat operator*(EFloat other) const;
	EFloat operator/(EFloat other) const;
	EFloat operator-() const;
	bool operator==(EFloat other) const;
	explicit operator float() const { return v; }
	explicit operator double() const { return ld; }

	void Check() const;
	float AbsoluteError() { return high - low; }
	float RelativeError() { return (float)(abs((ld - v) / ld)); }

private:
	float v;	// value
	float low, high;	// absolute error
	long double ld;		// long double value, for more precision.

	friend inline EFloat sqrt(EFloat other);
	friend inline EFloat abs(EFloat other);
	friend inline bool Quadratic(EFloat A, EFloat B, EFloat C, EFloat *t0, EFloat *t1);
};

// EFloat Inline Functions
inline EFloat operator*(float f, EFloat other) { return EFloat(f) * other; }

inline EFloat operator/(float f, EFloat other) { return EFloat(f) / other; }

inline EFloat operator+(float f, EFloat other) { return EFloat(f) + other; }

inline EFloat operator-(float f, EFloat other) { return EFloat(f) - other; }

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
	double delta = (double)B.v * (double)B.v - 4.0 * (double)A.v * double(C.v);
	if (delta < 0.0) 
		return false;
	double sqrtDelta = sqrt(delta);
	EFloat sqrtDeltaFlt((float)sqrtDelta, H_MACHINEEPSLION * (float)sqrtDelta);

	EFloat q;
	if ((float)B < 0) q = -0.5 * (B - sqrtDeltaFlt);
	else q = -0.5 * (B + sqrtDeltaFlt);
	*t0 = q / A;
	*t1 = C / q;
	if ((float)*t0 > (float)*t1) 
		swap(*t0, *t1);
	return true;
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

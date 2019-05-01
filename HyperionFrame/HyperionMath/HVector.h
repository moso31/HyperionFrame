#pragma once
#include "HMathBasic.h"

class HVector2
{
public:
	HVector2(HFloat value) : x(value), y(value) {}

	HFloat operator[] (const HInt& index) { return v[index]; }

public:
	union
	{
		struct { HFloat x, y; };
		struct { HFloat r, g; };
		HFloat v[2];
	};
};

class HVector3
{
public:
	HVector3() {}
	HVector3(HFloat value) : x(value), y(value), z(value) {}
	HVector3(HFloat x, HFloat y, HFloat z) : x(x), y(y), z(z) {}
	~HVector3() {}

	HVector3 operator+ (const HVector3& v) const;
	HVector3 operator+ (const HFloat& f) const;
	HVector3 operator- (const HVector3& v) const;
	HVector3 operator- (const HFloat& f) const;
	HVector3 operator- () const;
	HVector3 operator* (const HVector3& v) const;
	HVector3 operator* (const HFloat& f) const;
	HVector3 operator/ (const HVector3& v) const;
	HVector3 operator/ (const HFloat& f) const;
	friend HVector3 operator+ (const HFloat& f, const HVector3& v);
	friend HVector3 operator- (const HFloat& f, const HVector3& v);
	friend HVector3 operator* (const HFloat& f, const HVector3& v);
	friend HVector3 operator/ (const HFloat& f, const HVector3& v);
	HFloat operator[] (const HInt& index) { return v[index]; }

	HFloat Angle(const HVector3& v) const;
	HFloat AngleNormal(const HVector3& v) const;
	HFloat Dot(const HVector3& v) const;
	HVector3 Cross(const HVector3& v) const;
	HFloat LengthSq() const;
	HFloat Length() const;
	HVector3 Normalize() const;
	HVector3 TransformCoord(const HMatrix4x4& m) const;
	HVector3 TransformNormal(const HMatrix4x4& m) const;

	static HVector3 zero() { return HVector3(0.0f); }

public:
	union
	{
		struct { HFloat x, y, z; };
		struct { HFloat r, g, b; };
		HFloat v[3];
	};
};

class HVector4
{
public:
	HVector4() {}
	HVector4(HFloat value) : x(value), y(value), z(value), w(value) {}
	HVector4(HFloat x, HFloat y, HFloat z, HFloat w) : x(x), y(y), z(z), w(w) {}
	~HVector4() {}

	HVector4 operator+ (const HVector4& v) const;
	HVector4 operator+ (const HFloat& f) const;
	HVector4 operator- (const HVector4& v) const;
	HVector4 operator- (const HFloat& f) const;
	HVector4 operator- () const;
	HVector4 operator* (const HVector4& v) const;
	HVector4 operator* (const HFloat& f) const;
	HVector4 operator/ (const HVector4& v) const;
	HVector4 operator/ (const HFloat& f) const;
	friend HVector4 operator+ (const HFloat& f, const HVector4& v);
	friend HVector4 operator- (const HFloat& f, const HVector4& v);
	friend HVector4 operator* (const HFloat& f, const HVector4& v);
	friend HVector4 operator/ (const HFloat& f, const HVector4& v);
	HFloat operator[] (const HInt& index) { return v[index]; }

public:
	union
	{
		struct { HFloat x, y, z, w; };
		struct { HFloat r, g, b, a; };
		HFloat v[4];
	};
};

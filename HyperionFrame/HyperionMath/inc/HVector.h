#pragma once
#include "HMathBasic.h"

class HFloat2
{
public:
	HFloat2() {}
	HFloat2(HFloat value) : x(value), y(value) {}
	HFloat2(HFloat x, HFloat y) : x(x), y(y) {}
	~HFloat2() {}

	HFloat2 operator+ (const HFloat2& v) const;
	HFloat2 operator+ (const HFloat& f) const;
	HFloat2 operator- (const HFloat2& v) const;
	HFloat2 operator- (const HFloat& f) const;
	HFloat2 operator- () const;
	HFloat2 operator* (const HFloat2& v) const;
	HFloat2 operator* (const HFloat& f) const;
	HFloat2 operator/ (const HFloat2& v) const;
	HFloat2 operator/ (const HFloat& f) const;
	friend HFloat2 operator+ (const HFloat& f, const HFloat2& v);
	friend HFloat2 operator- (const HFloat& f, const HFloat2& v);
	friend HFloat2 operator* (const HFloat& f, const HFloat2& v);
	friend HFloat2 operator/ (const HFloat& f, const HFloat2& v);
	HFloat operator[] (const HInt& index) { return v[index]; }

	HFloat2 MaxVector(const HFloat2& v) const;
	HFloat2 MinVector(const HFloat2& v) const;
	HFloat2 Reciprocal() const;
	HFloat Angle(const HFloat2& v) const;
	HFloat AngleNormal(const HFloat2& v) const;
	HFloat Dot(const HFloat2& v) const;
	HFloat LengthSq() const;
	HFloat Length() const;
	HFloat2 Normalize() const;

	static HFloat2 zero() { return HFloat2(0.0f); }

public:
	union
	{
		struct { HFloat x, y; };
		struct { HFloat r, g; };
		HFloat v[2];
	};
};

class HFloat3
{
public:
	HFloat3() {}
	HFloat3(HFloat value) : x(value), y(value), z(value) {}
	HFloat3(HFloat x, HFloat y, HFloat z) : x(x), y(y), z(z) {}
	~HFloat3() {}

	HFloat3 operator+ (const HFloat3& v) const;
	HFloat3 operator+ (const HFloat& f) const;
	HFloat3 operator- (const HFloat3& v) const;
	HFloat3 operator- (const HFloat& f) const;
	HFloat3 operator- () const;
	HFloat3 operator* (const HFloat3& v) const;
	HFloat3 operator* (const HFloat& f) const;
	HFloat3 operator/ (const HFloat3& v) const;
	HFloat3 operator/ (const HFloat& f) const;
	friend HFloat3 operator+ (const HFloat& f, const HFloat3& v);
	friend HFloat3 operator- (const HFloat& f, const HFloat3& v);
	friend HFloat3 operator* (const HFloat& f, const HFloat3& v);
	friend HFloat3 operator/ (const HFloat& f, const HFloat3& v);
	HFloat operator[] (const HInt& index) { return v[index]; }

	HFloat3 MaxVector(const HFloat3& v) const;
	HFloat3 MinVector(const HFloat3& v) const;
	HFloat3 Reciprocal() const;
	HFloat Angle(const HFloat3& v) const;
	HFloat AngleNormal(const HFloat3& v) const;
	HFloat Dot(const HFloat3& v) const;
	HFloat3 Cross(const HFloat3& v) const;
	HFloat LengthSq() const;
	HFloat Length() const;
	HFloat3 Normalize() const;
	HFloat3 TransformCoord(const HMatrix4x4& m) const;
	HFloat3 TransformNormal(const HMatrix4x4& m) const;

public:
	union
	{
		struct { HFloat x, y, z; };
		struct { HFloat r, g, b; };
		HFloat v[3];
	};
};

class HFloat4
{
public:
	HFloat4() {}
	HFloat4(HFloat value) : x(value), y(value), z(value), w(value) {}
	HFloat4(HFloat x, HFloat y, HFloat z, HFloat w) : x(x), y(y), z(z), w(w) {}
	~HFloat4() {}

	HFloat4 operator+ (const HFloat4& v) const;
	HFloat4 operator+ (const HFloat& f) const;
	HFloat4 operator- (const HFloat4& v) const;
	HFloat4 operator- (const HFloat& f) const;
	HFloat4 operator- () const;
	HFloat4 operator* (const HFloat4& v) const;
	HFloat4 operator* (const HFloat& f) const;
	HFloat4 operator/ (const HFloat4& v) const;
	HFloat4 operator/ (const HFloat& f) const;
	friend HFloat4 operator+ (const HFloat& f, const HFloat4& v);
	friend HFloat4 operator- (const HFloat& f, const HFloat4& v);
	friend HFloat4 operator* (const HFloat& f, const HFloat4& v);
	friend HFloat4 operator/ (const HFloat& f, const HFloat4& v);
	HFloat operator[] (const HInt& index) { return v[index]; }

	HFloat4 MaxVector(const HFloat4& v) const;
	HFloat4 MinVector(const HFloat4& v) const;
	HFloat4 Reciprocal() const;

public:
	union
	{
		struct { HFloat x, y, z, w; };
		struct { HFloat r, g, b, a; };
		HFloat v[4];
	};
};

class HInt2
{
public:
	HInt2() {}
	HInt2(HInt value) : x(value), y(value) {}
	HInt2(HInt x, HInt y) : x(x), y(y) {}
	~HInt2() {}

public:
	union
	{
		struct { HInt x, y; };
		struct { HInt r, g; };
		HInt v[2];
	};
};

class HInt3
{
public:
	HInt3() {}
	HInt3(HInt value) : x(value), y(value), z(value) {}
	HInt3(HInt x, HInt y, HInt z) : x(x), y(y), z(z) {}
	~HInt3() {}

public:
	union
	{
		struct { HInt x, y, z; };
		struct { HInt r, g, b; };
		HInt v[3];
	};
};

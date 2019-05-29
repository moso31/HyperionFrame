#pragma once
#include "HMathBasic.h"

class HFloat2
{
public:
	HFloat2() = default;
	HFloat2(HFloat value);
	HFloat2(HFloat x, HFloat y);
	HFloat2(const HFloat2& v);
	HFloat2(const HFloat3& v);
	HFloat2(const HFloat4& v);
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
	HFloat2 operator+=(const HFloat2& v);
	HFloat2 operator+=(const HFloat& f);
	HFloat2 operator-=(const HFloat2& v);
	HFloat2 operator-=(const HFloat& f);
	HFloat2 operator*=(const HFloat& f);
	HFloat2 operator/=(const HFloat& f);
	HFloat& operator[] (const HInt& index) { return v[index]; }
	bool operator==(const HFloat2& v) const;
	bool operator!=(const HFloat2& v) const;

	HFloat2 MaxVector(const HFloat2& v) const;
	HFloat2 MinVector(const HFloat2& v) const;
	HFloat2 Reciprocal() const;
	HFloat Angle(const HFloat2& v) const;
	HFloat AngleNormal(const HFloat2& v) const;
	HFloat Dot(const HFloat2& v) const;
	HFloat LengthSq() const;
	HFloat Length() const;
	HFloat2 Abs() const;
	HFloat2 Sqrt() const;
	HFloat2 Normalize() const;
	HFloat2 Permute(const HInt x, const HInt y) const;
	HInt MaxDimension() const;
	HFloat MaxComponent() const;
	bool Equal(const HFloat2& v, HFloat tolerance = H_EPSILON);

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
	HFloat3() = default;
	HFloat3(HFloat value);
	HFloat3(HFloat x, HFloat y, HFloat z);
	HFloat3(const HFloat2& v, const HFloat z);
	HFloat3(const HFloat3& v);
	HFloat3(const HFloat4& v);
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
	HFloat3 operator+=(const HFloat3& v);
	HFloat3 operator+=(const HFloat& f);
	HFloat3 operator-=(const HFloat3& v);
	HFloat3 operator-=(const HFloat& f);
	HFloat3 operator*=(const HFloat& f);
	HFloat3 operator/=(const HFloat& f);
	HFloat& operator[] (const HInt& index) { return v[index]; }
	bool operator==(const HFloat3& v) const;
	bool operator!=(const HFloat3& v) const;

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
	HFloat3 Abs() const;
	HFloat3 Sqrt() const;
	//HFloat3 TransformCoord(const HFloat4x4& m) const;
	HFloat3 TransformCoord(const HFloat4x4& m, HFloat3& out_pError) const;
	HFloat3 TransformCoord(const HFloat4x4& m, const HFloat3& pError, HFloat3& out_pError) const;
	//HFloat3 TransformNormal(const HFloat4x4& m) const;
	HFloat3 TransformNormal(const HFloat4x4& m, HFloat3& out_pError) const;
	HFloat3 TransformNormal(const HFloat4x4& m, const HFloat3& pError, HFloat3& out_pError) const;
	HFloat3 Permute(const HInt x, const HInt y, const HInt z) const;
	HInt MaxDimension() const;
	HFloat MaxComponent() const;
	HFloat3 Clamp(const HFloat3& min, const HFloat3& max) const;
	bool Equal(const HFloat3& v, HFloat tolerance = H_EPSILON);

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
	HFloat4() = default;
	HFloat4(HFloat value);
	HFloat4(HFloat x, HFloat y, HFloat z, HFloat w);
	HFloat4(const HFloat2& v, const HFloat z, const HFloat w);
	HFloat4(const HFloat3& v, const HFloat w);
	HFloat4(const HFloat4& v);
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
	HFloat4 operator+=(const HFloat4& v);
	HFloat4 operator+=(const HFloat& f);
	HFloat4 operator-=(const HFloat4& v);
	HFloat4 operator-=(const HFloat& f);
	HFloat4 operator*=(const HFloat& f);
	HFloat4 operator/=(const HFloat& f);
	HFloat& operator[] (const HInt& index) { return v[index]; }
	bool operator==(const HFloat4& v) const;
	bool operator!=(const HFloat4& v) const;

	HFloat4 MaxVector(const HFloat4& v) const;
	HFloat4 MinVector(const HFloat4& v) const;
	HFloat4 Reciprocal() const;
	HFloat4 Abs() const;
	HFloat4 Sqrt() const;
	HFloat4 Transform(const HFloat4x4& m, HFloat4& out_pError) const;
	HFloat4 Permute(const HInt x, const HInt y, const HInt z, const HInt w) const;
	bool Equal(const HFloat4& v, HFloat tolerance = H_EPSILON);

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
	HInt2() = default;
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
	HInt3() = default;
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

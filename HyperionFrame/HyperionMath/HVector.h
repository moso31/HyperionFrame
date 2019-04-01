#pragma once
#include <DirectXMath.h>
#include "HMathType.h"

class HVector2
{
public:
	HVector2(HFloat value = 0.0f) : x(value), y(value) {}

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
	HVector3(HFloat value = 0.0f) : x(value), y(value), z(value) {};
	HVector3(HFloat x, HFloat y, HFloat z) : x(x), y(y), z(z) {};
	~HVector3() {};

	HVector3 operator+ (const HVector3& v);
	HVector3 operator+ (const HFloat& f);
	HVector3 operator- (const HVector3& v);
	HVector3 operator- (const HFloat& f);
	HVector3 operator* (const HVector3& v);
	HVector3 operator* (const HFloat& f);
	HVector3 operator/ (const HVector3& v);
	HVector3 operator/ (const HFloat& f);
	HFloat operator[] (const HInt& index) { return v[index]; }

	HFloat Dot(const HVector3& v);
	HVector3 Cross(const HVector3& v);
	HFloat LengthSq();
	HFloat Length();
	HVector3 Normalize();
	HVector3 TransformCoord(const HMatrix4x4& m);
	HVector3 TransformNormal(const HMatrix4x4& m);

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
	HVector4(HFloat value = 0.0f) : x(value), y(value), z(value), w(value) {}

public:
	union
	{
		struct { HFloat x, y, z, w; };
		struct { HFloat r, g, b, a; };
		HFloat v[4];
	};
};

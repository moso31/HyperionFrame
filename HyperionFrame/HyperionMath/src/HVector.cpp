#pragma once
#include "HVector.h"
#include "HMatrix.h"

HFloat2::HFloat2(HFloat value) :
	x(value), y(value)
{
}

HFloat2::HFloat2(HFloat x, HFloat y) :
	x(x), y(y)
{
}

HFloat2::HFloat2(const HFloat2 & v) :
	x(v.x), y(v.y)
{
}

HFloat2::HFloat2(const HFloat3 & v) :
	x(v.x), y(v.y)
{
}

HFloat2::HFloat2(const HFloat4 & v) :
	x(v.x), y(v.y)
{
}

HFloat2 HFloat2::operator+(const HFloat2 & v) const
{
	return HFloat2(x + v.x, y + v.y);
}

HFloat2 HFloat2::operator+(const HFloat & f) const
{
	return HFloat2(x + f, y + f);
}

HFloat2 HFloat2::operator-(const HFloat2 & v) const
{
	return HFloat2(x - v.x, y - v.y);
}

HFloat2 HFloat2::operator-(const HFloat & f) const
{
	return HFloat2(x - f, y - f);
}

HFloat2 HFloat2::operator-() const
{
	return HFloat2(-x, -y);
}

HFloat2 HFloat2::operator*(const HFloat2 & v) const
{
	return HFloat2(x * v.x, y * v.y);
}

HFloat2 HFloat2::operator*(const HFloat & f) const
{
	return HFloat2(x * f, y * f);
}

HFloat2 HFloat2::operator/(const HFloat2 & v) const
{
	return HFloat2(x / v.x, y / v.y);
}

HFloat2 HFloat2::operator/(const HFloat & f) const
{
	HFloat d = 1.0f / f;
	return HFloat2(x * d, y * d);
}

HFloat2 operator+(const HFloat & f, const HFloat2 & v)
{
	return v + f;
}

HFloat2 operator-(const HFloat & f, const HFloat2 & v)
{
	return HFloat2(f - v.x, f - v.y);
}

HFloat2 operator*(const HFloat & f, const HFloat2 & v)
{
	return v * f;
}

HFloat2 operator/(const HFloat & f, const HFloat2 & v)
{
	return HFloat2(f / v.x, f / v.y);
}

HFloat2 HFloat2::operator+=(const HFloat2 & v)
{
	x += v.x;
	y += v.y;
	return *this;
}

HFloat2 HFloat2::operator+=(const HFloat & f)
{
	x += f;
	y += f;
	return *this;
}

HFloat2 HFloat2::operator-=(const HFloat2 & v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

HFloat2 HFloat2::operator-=(const HFloat & f)
{
	x -= f;
	y -= f;
	return *this;
}

HFloat2 HFloat2::operator*=(const HFloat & f)
{
	x *= f;
	y *= f;
	return *this;
}

HFloat2 HFloat2::operator/=(const HFloat & f)
{
	float invf = 1.0f / f;
	x *= invf;
	y *= invf;
	return *this;
}

bool HFloat2::operator==(const HFloat2 & v) const
{
	return x == v.x && y == v.y;
}

bool HFloat2::operator!=(const HFloat2 & v) const
{
	return x != v.x || y != v.y;
}

HFloat2 HFloat2::MaxVector(const HFloat2 & v) const
{
	return HFloat2(
		x > v.x ? x : v.x,
		y > v.y ? y : v.y
	);
}

HFloat2 HFloat2::MinVector(const HFloat2 & v) const
{
	return HFloat2(
		x < v.x ? x : v.x,
		y < v.y ? y : v.y
	);
}

HFloat2 HFloat2::Reciprocal() const
{
	return HFloat2(1.0f / x, 1.0f / y);
}

HFloat HFloat2::Angle(const HFloat2 & v) const
{
	HFloat cosAngle = Dot(v) / (Length() * v.Length());
	HFloat result = Clamp(cosAngle, -1.0f, 1.0f);
	result = acosf(result);
	return result;
}

HFloat HFloat2::AngleNormal(const HFloat2 & v) const
{
	HFloat cosAngle = Dot(v);
	HFloat result = Clamp(cosAngle, -1.0f, 1.0f);
	result = acosf(result);
	return result;
}

HFloat HFloat2::Dot(const HFloat2 & v) const
{
	return x * v.x + y * v.y;
}

HFloat HFloat2::LengthSq() const
{
	return x * x + y * y;
}

HFloat HFloat2::Length() const
{
	return sqrtf(x * x + y * y);
}

HFloat2 HFloat2::Abs() const
{
	return HFloat2(fabsf(x), fabsf(y));
}

HFloat2 HFloat2::Sqrt() const
{
	return HFloat2(sqrtf(x), sqrtf(y));
}

HFloat2 HFloat2::Normalize() const
{
	HFloat dt = 1.0f / Length();
	return HFloat2(x * dt, y * dt);
}

HFloat2 HFloat2::Permute(const HInt x, const HInt y) const
{
	return HFloat2(v[x], v[y]);
}

HInt HFloat2::MaxDimension() const
{
	return x > y ? 0 : 1;
}

HFloat HFloat2::MaxComponent() const
{
	return max(x, y);
}

bool HFloat2::Equal(const HFloat2 & v, HFloat tolerance)
{
	return (fabsf(x - v.x) < tolerance &&
		fabsf(y - v.y) < tolerance);
}

HFloat3::HFloat3(HFloat value) :
	x(value), y(value), z(value)
{
}

HFloat3::HFloat3(HFloat x, HFloat y, HFloat z) :
	x(x), y(y), z(z)
{
}

HFloat3::HFloat3(const HFloat2 & v, const HFloat z) :
	x(v.x), y(v.y), z(z) 
{
}

HFloat3::HFloat3(const HFloat3 & v) :
	x(v.x), y(v.y), z(v.z)
{
}

HFloat3::HFloat3(const HFloat4 & v) :
	x(v.x), y(v.y), z(v.z)
{
}

HFloat3 HFloat3::operator+(const HFloat3 & v) const
{
	return HFloat3(x + v.x, y + v.y, z + v.z);
}

HFloat3 HFloat3::operator+(const HFloat & f) const
{
	return HFloat3(x + f, y + f, z + f);
}

HFloat3 HFloat3::operator-(const HFloat3 & v) const
{
	return HFloat3(x - v.x, y - v.y, z - v.z);
}

HFloat3 HFloat3::operator-(const HFloat & f) const
{
	return HFloat3(x - f, y - f, z - f);
}

HFloat3 HFloat3::operator-() const
{
	return HFloat3(-x, -y, -z);
}

HFloat3 HFloat3::operator*(const HFloat3 & v) const
{
	return HFloat3(x * v.x, y * v.y, z * v.z);
}

HFloat3 HFloat3::operator*(const HFloat & f) const
{
	return HFloat3(x * f, y * f, z * f);
}

HFloat3 HFloat3::operator/(const HFloat3 & v) const
{
	return HFloat3(x / v.x, y / v.y, z / v.z);
}

HFloat3 HFloat3::operator/(const HFloat & f) const
{
	HFloat d = 1.0f / f;
	return HFloat3(x * d, y * d, z * d);
}

HFloat3 HFloat3::operator+=(const HFloat3 & v)
{
	x += v.x; 
	y += v.y;
	z += v.z;
	return *this;
}

HFloat3 HFloat3::operator+=(const HFloat & f)
{
	x += f;
	y += f;
	z += f;
	return *this;
}

HFloat3 HFloat3::operator-=(const HFloat3 & v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

HFloat3 HFloat3::operator-=(const HFloat & f)
{
	x -= f;
	y -= f;
	z -= f;
	return *this;
}

HFloat3 HFloat3::operator*=(const HFloat & f)
{
	x *= f;
	y *= f;
	z *= f;
	return *this;
}

HFloat3 HFloat3::operator/=(const HFloat & f)
{
	float invf = 1.0f / f;
	x *= invf;
	y *= invf;
	z *= invf;
	return *this;
}

bool HFloat3::operator==(const HFloat3 & v) const
{
	return x == v.x && y == v.y && z == v.z;
}

bool HFloat3::operator!=(const HFloat3 & v) const
{
	return x != v.x || y != v.y || z != v.z;
}

HFloat3 operator+(const HFloat & f, const HFloat3 & v)
{
	return v + f;
}

HFloat3 operator-(const HFloat & f, const HFloat3 & v)
{
	return HFloat3(f - v.x, f - v.y, f - v.z);
}

HFloat3 operator*(const HFloat & f, const HFloat3 & v)
{
	return v * f;
}

HFloat3 operator/(const HFloat & f, const HFloat3 & v)
{
	return HFloat3(f / v.x, f / v.y, f / v.z);
}

HFloat3 HFloat3::MaxVector(const HFloat3 & v) const
{
	return HFloat3(
		x > v.x ? x : v.x,
		y > v.y ? y : v.y,
		z > v.z ? z : v.z
	);
}

HFloat3 HFloat3::MinVector(const HFloat3 & v) const
{
	return HFloat3(
		x < v.x ? x : v.x,
		y < v.y ? y : v.y,
		z < v.z ? z : v.z
	);
}

HFloat3 HFloat3::Reciprocal() const
{
	return HFloat3(1.0f / x, 1.0f / y, 1.0f / z);
}

HFloat HFloat3::Angle(const HFloat3 & v) const
{
	HFloat cosAngle = Dot(v) / (Length() * v.Length());
	HFloat result = ::Clamp(cosAngle, -1.0f, 1.0f);
	result = acosf(result);
	return result;
}

HFloat HFloat3::AngleNormal(const HFloat3 & v) const
{
	HFloat cosAngle = Dot(v);
	HFloat result = ::Clamp(cosAngle, -1.0f, 1.0f);
	result = acosf(result);
	return result;
}

HFloat HFloat3::Dot(const HFloat3 & v) const
{
	return x * v.x + y * v.y + z * v.z;
}

HFloat3 HFloat3::Cross(const HFloat3 & v) const
{
	return HFloat3(y * v.z - v.y * z, v.x * z - x * v.z, x * v.y - v.x * y);
}

HFloat HFloat3::LengthSq() const
{
	return x * x + y * y + z * z;
}

HFloat HFloat3::Length() const
{
	return sqrtf(x * x + y * y + z * z);
}

HFloat3 HFloat3::Normalize() const
{
	HFloat dt = 1.0f / Length();
	return HFloat3(x * dt, y * dt, z * dt);
}

HFloat3 HFloat3::Abs() const
{
	return HFloat3(fabsf(x), fabsf(y), fabsf(z));
}

HFloat3 HFloat3::Sqrt() const
{
	return HFloat3(sqrtf(x), sqrtf(y), sqrtf(z));
}

HFloat3 HFloat3::TransformCoord(const HFloat4x4 & m) const
{
	return HFloat3(m._11 * x + m._12 * y + m._13 * z + m._14,
			m._21 * x + m._22 * y + m._23 * z + m._24,
			m._31 * x + m._32 * y + m._33 * z + m._34);
}

HFloat3 HFloat3::TransformNormal(const HFloat4x4 & m) const
{
	return HFloat3(m._11 * x + m._12 * y + m._13 * z,
		m._21 * x + m._22 * y + m._23 * z,
		m._31 * x + m._32 * y + m._33 * z);
}

HFloat3 HFloat3::Permute(const HInt x, const HInt y, const HInt z) const
{
	return HFloat3(v[x], v[y], v[z]);
}

HInt HFloat3::MaxDimension() const
{
	return (x > y) ? ((x > z) ? 0 : 2) : ((y > z) ? 1 : 2);
}

HFloat HFloat3::MaxComponent() const
{
	return max(x, max(y, z));
}

HFloat3 HFloat3::Clamp(const HFloat3 & min, const HFloat3 & max) const
{
	return MaxVector(min).MinVector(max);
}

bool HFloat3::Equal(const HFloat3 & v, HFloat tolerance)
{
	return (fabsf(x - v.x) < tolerance &&
		fabsf(y - v.y) < tolerance &&
		fabsf(z - v.z) < tolerance);
}

HFloat4::HFloat4(HFloat value) :
	x(value), y(value), z(value), w(value)
{
}

HFloat4::HFloat4(HFloat x, HFloat y, HFloat z, HFloat w) :
	x(x), y(y), z(z), w(w)
{
}

HFloat4::HFloat4(const HFloat2 & v, const HFloat z, const HFloat w)
	: x(v.x), y(v.y), z(z), w(w) 
{
}

HFloat4::HFloat4(const HFloat3 & v, const HFloat w) : 
	x(v.x), y(v.y), z(v.z), w(w)
{
}

HFloat4::HFloat4(const HFloat4 & v) :
	x(v.x), y(v.y), z(v.z), w(v.w)
{
}

HFloat4 HFloat4::operator+(const HFloat4 & v) const
{
	return HFloat4(x + v.x, y + v.y, z + v.z, w + v.w);
}

HFloat4 HFloat4::operator+(const HFloat & f) const
{
	return HFloat4(x + f, y + f, z + f, w + f);
}

HFloat4 HFloat4::operator-(const HFloat4 & v) const
{
	return HFloat4(x - v.x, y - v.y, z - v.z, w - v.w);
}

HFloat4 HFloat4::operator-(const HFloat & f) const
{
	return HFloat4(x - f, y - f, z - f, w - f);
}

HFloat4 HFloat4::operator-() const
{
	return HFloat4(-x, -y, -z, -w);
}

HFloat4 HFloat4::operator*(const HFloat4 & v) const
{
	return HFloat4(x + v.x, y + v.y, z + v.z, w + v.w);
}

HFloat4 HFloat4::operator*(const HFloat & f) const
{
	return HFloat4(x * f, y * f, z * f, w * f);
}

HFloat4 HFloat4::operator/(const HFloat4 & v) const
{
	return HFloat4(x / v.x, y / v.y, z / v.z, w / v.w);
}

HFloat4 HFloat4::operator/(const HFloat & f) const
{
	HFloat d = 1.0f / f;
	return HFloat4(x * d, y * d, z * d, w * d);
}

HFloat4 operator+(const HFloat & f, const HFloat4 & v)
{
	return v + f;
}

HFloat4 operator-(const HFloat & f, const HFloat4 & v)
{
	return HFloat4(f - v.x, f - v.y, f - v.z, f - v.w);
}

HFloat4 operator*(const HFloat & f, const HFloat4 & v)
{
	return v * f;
}

HFloat4 operator/(const HFloat & f, const HFloat4 & v)
{
	return HFloat4(f / v.x, f / v.y, f / v.z, f / v.w);
}

HFloat4 HFloat4::operator+=(const HFloat4 & v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}

HFloat4 HFloat4::operator+=(const HFloat & f)
{
	x += f;
	y += f;
	z += f;
	w += f;
	return *this;
}

HFloat4 HFloat4::operator-=(const HFloat4 & v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return *this;
}

HFloat4 HFloat4::operator-=(const HFloat & f)
{
	x -= f;
	y -= f;
	z -= f;
	w -= f;
	return *this;
}

HFloat4 HFloat4::operator*=(const HFloat & f)
{
	x *= f;
	y *= f;
	z *= f;
	w *= f;
	return *this;
}

HFloat4 HFloat4::operator/=(const HFloat & f)
{
	float invf = 1.0f / f;
	x *= invf;
	y *= invf;
	z *= invf;
	w *= invf;
	return *this;
}

bool HFloat4::operator==(const HFloat4 & v) const
{
	return x == v.x && y == v.y && z == v.z && w == v.w;
}

bool HFloat4::operator!=(const HFloat4 & v) const
{
	return x != v.x || y != v.y || z != v.z || w != v.w;
}

HFloat4 HFloat4::MaxVector(const HFloat4 & v) const
{
	return HFloat4(
		x > v.x ? x : v.x,
		y > v.y ? y : v.y,
		z > v.z ? z : v.z,
		w > v.w ? w : v.w
	);
}

HFloat4 HFloat4::MinVector(const HFloat4 & v) const
{
	return HFloat4(
		x < v.x ? x : v.x,
		y < v.y ? y : v.y,
		z < v.z ? z : v.z,
		w < v.w ? w : v.w
	);
}

HFloat4 HFloat4::Reciprocal() const
{
	return HFloat4(1.0f / x, 1.0f / y, 1.0f / z, 1.0f / w);
}

HFloat4 HFloat4::Abs() const
{
	return HFloat4(fabsf(x), fabsf(y), fabsf(z), fabsf(w));
}

HFloat4 HFloat4::Sqrt() const
{
	return HFloat4(sqrtf(x), sqrtf(y), sqrtf(z), sqrtf(w));
}

HFloat4 HFloat4::Transform(const HFloat4x4 & m) const
{
	return HFloat4(
		m._11 * v[0] + m._12 * v[1] + m._13 * v[2] + m._14 * v[3],
		m._21 * v[0] + m._22 * v[1] + m._23 * v[2] + m._24 * v[3],
		m._31 * v[0] + m._32 * v[1] + m._33 * v[2] + m._34 * v[3],
		m._41 * v[0] + m._42 * v[1] + m._43 * v[2] + m._44 * v[3]
	);
}

HFloat4 HFloat4::Permute(const HInt x, const HInt y, const HInt z, const HInt w) const
{
	return HFloat4(v[x], v[y], v[z], v[w]);
}

bool HFloat4::Equal(const HFloat4 & v, HFloat tolerance)
{
	return (fabsf(x - v.x) < tolerance &&
		fabsf(y - v.y) < tolerance &&
		fabsf(z - v.z) < tolerance &&
		fabsf(w - v.w) < tolerance);
}

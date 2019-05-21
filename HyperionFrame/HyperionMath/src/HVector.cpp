#include "HVector.h"
#include "HMatrix.h"
#pragma once

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

HFloat2 HFloat2::Normalize() const
{
	HFloat dt = 1.0f / Length();
	return HFloat2(x * dt, y * dt);
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
	HFloat result = Clamp(cosAngle, -1.0f, 1.0f);
	result = acosf(result);
	return result;
}

HFloat HFloat3::AngleNormal(const HFloat3 & v) const
{
	HFloat cosAngle = Dot(v);
	HFloat result = Clamp(cosAngle, -1.0f, 1.0f);
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

HFloat3 HFloat3::TransformCoord(const HMatrix4x4 & m) const
{
	return HFloat3(m._11 * x + m._12 * y + m._13 * z + m._14,
			m._21 * x + m._22 * y + m._23 * z + m._24,
			m._31 * x + m._32 * y + m._33 * z + m._34);
}

HFloat3 HFloat3::TransformNormal(const HMatrix4x4 & m) const
{
	return HFloat3(m._11 * x + m._12 * y + m._13 * z,
		m._21 * x + m._22 * y + m._23 * z,
		m._31 * x + m._32 * y + m._33 * z);
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

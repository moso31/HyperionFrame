#include "HVector.h"
#include "HMatrix.h"
#pragma once

HVector3 HVector3::operator+(const HVector3 & v) const
{
	return HVector3(x + v.x, y + v.y, z + v.z);
}

HVector3 HVector3::operator+(const HFloat & f) const
{
	return HVector3(x + f, y + f, z + f);
}

HVector3 HVector3::operator-(const HVector3 & v) const
{
	return HVector3(x - v.x, y - v.y, z - v.z);
}

HVector3 HVector3::operator-(const HFloat & f) const
{
	return HVector3(x - f, y - f, z - f);
}

HVector3 HVector3::operator-() const
{
	return HVector3(-x, -y, -z);
}

HVector3 HVector3::operator*(const HVector3 & v) const
{
	return HVector3(x * v.x, y * v.y, z * v.z);
}

HVector3 HVector3::operator*(const HFloat & f) const
{
	return HVector3(x * f, y * f, z * f);
}

HVector3 HVector3::operator/(const HVector3 & v) const
{
	return HVector3(x / v.x, y / v.y, z / v.z);
}

HVector3 HVector3::operator/(const HFloat & f) const
{
	HFloat d = 1.0f / f;
	return HVector3(x * d, y * d, z * d);
}

HVector3 operator+(const HFloat & f, const HVector3 & v)
{
	return v + f;
}

HVector3 operator-(const HFloat & f, const HVector3 & v)
{
	return HVector3(f - v.x, f - v.y, f - v.z);
}

HVector3 operator*(const HFloat & f, const HVector3 & v)
{
	return v * f;
}

HVector3 operator/(const HFloat & f, const HVector3 & v)
{
	return HVector3(f / v.x, f / v.y, f / v.z);
}

HFloat HVector3::Angle(const HVector3 & v) const
{
	HFloat cosAngle = Dot(v) / (Length() * v.Length());
	HFloat result = Clamp(cosAngle, -1.0f, 1.0f);
	result = acosf(result);
	return result;
}

HFloat HVector3::AngleNormal(const HVector3 & v) const
{
	HFloat cosAngle = Dot(v);
	HFloat result = Clamp(cosAngle, -1.0f, 1.0f);
	result = acosf(result);
	return result;
}

HFloat HVector3::Dot(const HVector3 & v) const
{
	return x * v.x + y * v.y + z * v.z;
}

HVector3 HVector3::Cross(const HVector3 & v) const
{
	return HVector3(y * v.z - v.y * z, v.x * z - x * v.z, x * v.y - v.x * y);
}

HFloat HVector3::LengthSq() const
{
	return x * x + y * y + z * z;
}

HFloat HVector3::Length() const
{
	return sqrtf(x * x + y * y + z * z);
}

HVector3 HVector3::Normalize() const
{
	HFloat dt = 1.0f / Length();
	return HVector3(x * dt, y * dt, z * dt);
}

HVector3 HVector3::TransformCoord(const HMatrix4x4 & m) const
{
	return HVector3(m._11 * x + m._12 * y + m._13 * z + m._14,
			m._21 * x + m._22 * y + m._23 * z + m._24,
			m._31 * x + m._32 * y + m._33 * z + m._33);
}

HVector3 HVector3::TransformNormal(const HMatrix4x4 & m) const
{
	return HVector3(m._11 * x + m._12 * y + m._13 * z,
		m._21 * x + m._22 * y + m._23 * z,
		m._31 * x + m._32 * y + m._33 * z);
}

HVector4 HVector4::operator+(const HVector4 & v) const
{
	return HVector4(x + v.x, y + v.y, z + v.z, w + v.w);
}

HVector4 HVector4::operator+(const HFloat & f) const
{
	return HVector4(x + f, y + f, z + f, w + f);
}

HVector4 HVector4::operator-(const HVector4 & v) const
{
	return HVector4(x - v.x, y - v.y, z - v.z, w - v.w);
}

HVector4 HVector4::operator-(const HFloat & f) const
{
	return HVector4(x - f, y - f, z - f, w - f);
}

HVector4 HVector4::operator-() const
{
	return HVector4(-x, -y, -z, -w);
}

HVector4 HVector4::operator*(const HVector4 & v) const
{
	return HVector4(x + v.x, y + v.y, z + v.z, w + v.w);
}

HVector4 HVector4::operator*(const HFloat & f) const
{
	return HVector4(x * f, y * f, z * f, w * f);
}

HVector4 HVector4::operator/(const HVector4 & v) const
{
	return HVector4(x / v.x, y / v.y, z / v.z, w / v.w);
}

HVector4 HVector4::operator/(const HFloat & f) const
{
	HFloat d = 1.0f / f;
	return HVector4(x * d, y * d, z * d, w * d);
}

HVector4 operator+(const HFloat & f, const HVector4 & v)
{
	return v + f;
}

HVector4 operator-(const HFloat & f, const HVector4 & v)
{
	return HVector4(f - v.x, f - v.y, f - v.z, f - v.w);
}

HVector4 operator*(const HFloat & f, const HVector4 & v)
{
	return v * f;
}

HVector4 operator/(const HFloat & f, const HVector4 & v)
{
	return HVector4(f / v.x, f / v.y, f / v.z, f / v.w);
}
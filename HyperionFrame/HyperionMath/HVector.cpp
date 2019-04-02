#include "HVector.h"
#pragma once

HVector3 HVector3::operator+(const HVector3 & v)
{
	return HVector3(x + v.x, y + v.y, z + v.z);
}

HVector3 HVector3::operator+(const HFloat & f)
{
	return HVector3(x + f, y + f, z + f);
}

HVector3 HVector3::operator-(const HVector3 & v)
{
	return HVector3(x - v.x, y - v.y, z - v.z);
}

HVector3 HVector3::operator-(const HFloat & f)
{
	return HVector3(x - f, y - f, z - f);
}

HVector3 HVector3::operator*(const HVector3 & v)
{
	return HVector3(x * v.x, y * v.y, z * v.z);
}

HVector3 HVector3::operator*(const HFloat & f)
{
	return HVector3(x * f, y * f, z * f);
}

HVector3 HVector3::operator/(const HVector3 & v)
{
	return HVector3(x / v.x, y / v.y, z / v.z);
}

HVector3 HVector3::operator/(const HFloat & f)
{
	HFloat d = 1.0f / f;
	return HVector3(x * d, y * d, z * d);
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

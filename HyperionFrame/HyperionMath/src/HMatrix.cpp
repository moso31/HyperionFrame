#include "HMatrix.h"
#include "HQuaternion.h"

HFloat4x4::HFloat4x4(HFloat _11, HFloat _12, HFloat _13, HFloat _14, HFloat _21, HFloat _22, HFloat _23, HFloat _24, HFloat _31, HFloat _32, HFloat _33, HFloat _34, HFloat _41, HFloat _42, HFloat _43, HFloat _44) :
	_11(_11), _12(_12), _13(_13), _14(_14),
	_21(_21), _22(_22), _23(_23), _24(_24),
	_31(_31), _32(_32), _33(_33), _34(_34),
	_41(_41), _42(_42), _43(_43), _44(_44)
{
}

HFloat4x4::HFloat4x4(const HFloat4x4 & m) :
	_11(m._11), _12(m._12), _13(m._13), _14(m._14),
	_21(m._21), _22(m._22), _23(m._23), _24(m._24),
	_31(m._31), _32(m._32), _33(m._33), _34(m._34),
	_41(m._41), _42(m._42), _43(m._43), _44(m._44)
{
}

HFloat4x4 HFloat4x4::operator+(const HFloat4x4 & m) const
{
	HFloat4x4 r;
	for (int i = 0; i < 4; i++)
		r[i] = v[i] + m.v[i];
	return r;
}

HFloat4x4 HFloat4x4::operator-(const HFloat4x4 & m) const
{
	HFloat4x4 r;
	for (int i = 0; i < 4; i++)
		r[i] = v[i] - m.v[i];
	return r;
}

HFloat4x4 HFloat4x4::operator*(const HFloat4x4 & m) const
{
	HFloat4x4 r;
	r.SetZero();
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				r.v[i].v[j] += v[i].v[k] * m.v[k].v[j];
			}
		}
	}
	return r;
}

void HFloat4x4::SetIdentity()
{
	_11 = 1.0f; _12 = 0.0f; _13 = 0.0f; _14 = 0.0f;
	_21 = 0.0f; _22 = 1.0f; _23 = 0.0f; _24 = 0.0f;
	_31 = 0.0f; _32 = 0.0f; _33 = 1.0f; _33 = 0.0f;
	_41 = 0.0f; _42 = 0.0f; _43 = 0.0f; _44 = 1.0f;
}

void HFloat4x4::SetZero()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			v[i].v[j] = 0.0f;
	}
}

void HFloat4x4::SetNaN()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			v[i].v[j] = NAN;
	}
}

HFloat4x4 HFloat4x4::Set(const HFloat m11, const HFloat m12, const HFloat m13, const HFloat m14, const HFloat m21, const HFloat m22, const HFloat m23, const HFloat m24, const HFloat m31, const HFloat m32, const HFloat m33, const HFloat m34, const HFloat m41, const HFloat m42, const HFloat m43, const HFloat m44)
{
	_11 = m11; _12 = m12; _13 = m13; _14 = m14;
	_21 = m21; _22 = m22; _23 = m23; _24 = m24;
	_31 = m31; _32 = m32; _33 = m33; _34 = m34;
	_41 = m41; _42 = m42; _43 = m43; _44 = m44;
	return *this;
}

HFloat4x4 HFloat4x4::SetTranslation(HFloat x, HFloat y, HFloat z)
{
	return Set(1.0f, 0.0f, 0.0f, x,
		0.0f, 1.0f, 0.0f, y,
		0.0f, 0.0f, 1.0f, z,
		0.0f, 0.0f, 0.0f, 1.0f);
}

HFloat4x4 HFloat4x4::SetTranslation(HFloat3 v)
{
	return SetTranslation(v.x, v.y, v.z);
}

HFloat4x4 HFloat4x4::SetRotationAxis(const HFloat3 & axis, HFloat angle)
{
	return SetRotationNormal(axis.Normalize(), angle);
}

HFloat4x4 HFloat4x4::SetRotationNormal(const HFloat3 & normal, HFloat angle)
{
	HFloat c = cosf(angle);
	HFloat s = sinf(angle);
	HFloat t = 1.0f - c;
	HFloat t1 = normal.x * normal.y * t;
	HFloat t2 = normal.z * s;
	HFloat t3 = normal.x * normal.z * t;
	HFloat t4 = normal.y * s;
	HFloat t5 = normal.y * normal.z * t;
	HFloat t6 = normal.x * s;

	return Set(c + normal.x * normal.x * t, t1 - t2, t3 + t4, 0.0f,
		t1 + t2, c + normal.y * normal.y * t, t5 - t6, 0.0f,
		t3 - t4, t5 + t6, c + normal.z * normal.z * t, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

HFloat4x4 HFloat4x4::SetRotationQuaternion(const HQuaternion & q)
{
	HFloat sqw = q.w * q.w;
	HFloat sqx = q.x * q.x;
	HFloat sqy = q.y * q.y;
	HFloat sqz = q.z * q.z;
	HFloat t1 = q.x * q.y;
	HFloat t2 = q.z * q.w;
	HFloat t3 = q.x * q.z;
	HFloat t4 = q.y * q.w;
	HFloat t5 = q.y * q.z;
	HFloat t6 = q.x * q.w;
	HFloat invs = 1 / (sqx + sqy + sqz + sqw);

	return Set((sqx - sqy - sqz + sqw) * invs, 2.0f * (t1 - t2) * invs, 2.0f * (t3 + t4) * invs, 0.0f,
		2.0f * (t1 + t2) * invs, (-sqx + sqy - sqz + sqw) * invs, 2.0f * (t5 - t6) * invs, 0.0f,
		2.0f * (t3 - t4) * invs, 2.0f * (t5 + t6) * invs, (-sqx - sqy + sqz + sqw) * invs, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

HFloat4x4 HFloat4x4::SetRotationX(HFloat angle)
{
	HFloat s = sinf(angle);
	HFloat c = cosf(angle);
	return Set(1, 0, 0, 0,
		0, c, -s, 0,
		0, s, c, 0,
		0, 0, 0, 1);
}

HFloat4x4 HFloat4x4::SetRotationY(HFloat angle)
{
	HFloat s = sinf(angle);
	HFloat c = cosf(angle);
	return Set(c, 0, s, 0,
		0, 1, 0, 0,
		-s, 0, c, 0,
		0, 0, 0, 1);
}

HFloat4x4 HFloat4x4::SetRotationZ(HFloat angle)
{
	HFloat s = sinf(angle);
	HFloat c = cosf(angle);
	return Set(c, -s, 0, 0,
		s, c, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
}

HFloat4x4 HFloat4x4::SetRotationXYZ(HFloat pitch, HFloat yaw, HFloat roll)
{
	HFloat cx = cosf(pitch);
	HFloat cy = cosf(yaw);
	HFloat cz = cosf(roll);
	HFloat sx = sinf(pitch);
	HFloat sy = sinf(yaw);
	HFloat sz = sinf(roll);
	HFloat cxcz = cx * cz;
	HFloat sxsz = sx * sz;
	HFloat sxcz = sx * cz;
	return Set(cy * cz, -cy * sz, sy, 0.0f,
		cx * sz + sxcz * sy, cxcz - sxsz * sy, -sx * cy, 0.0f,
		sxsz - cxcz * sy, sxcz + cx * sy * sz, cx * cy, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

HFloat4x4 HFloat4x4::SetRotationXYZ(HFloat3 v)
{
	return SetRotationXYZ(v.x, v.y, v.z);
}

HFloat4x4 HFloat4x4::SetScale(HFloat x, HFloat y, HFloat z)
{
	return Set(x, 0.0f, 0.0f, 0.0f,
		0.0f, y, 0.0f, 0.0f,
		0.0f, 0.0f, z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

HFloat4x4 HFloat4x4::SetScale(HFloat3 v)
{
	return SetScale(v.x, v.y, v.z);
}

HFloat4x4 HFloat4x4::Transpose() const
{
	return HFloat4x4(_11, _21, _31, _41,
		_12, _22, _32, _42,
		_13, _23, _33, _43,
		_14, _24, _34, _44);
}

HFloat4x4 HFloat4x4::Inverse() const
{
	int b[4] = { 0, 0, 0, 0 };
	int sq[4] = { 0, 1, 2, 3 };
	HFloat4x4 mCopy(*this);
	HFloat4x4 result;
	for (int i = 0; i < 4; i++)
	{
		int ix = 0, iy = 0;
		HFloat big = 0.0f;
		for (int j = 0; j < 4; j++)
		{
			if (b[j] != 1)
			{
				for (int k = 0; k < 4; k++)
				{
					HFloat val = fabsf(mCopy[j][k]);
					if (b[k] == 0 && val > big)
					{
						big = val;
						ix = j;
						iy = k;
					}
					else if (b[k] > 1)
					{
						result.SetNaN();
						return result;
					}
				}
			}
		}
		b[iy]++;

		int sqt = sq[ix];
		sq[ix] = sq[iy];
		sq[iy] = sqt;
		if (ix != iy) {
			for (int k = 0; k < 4; ++k)
			{
				HFloat t = mCopy[ix][k];
				mCopy.v[ix].v[k] = mCopy[iy][k];
				mCopy.v[iy].v[k] = t;
			}
		}

		if (mCopy[iy][iy] == 0.0f)
		{
			result.SetNaN();
			return result;
		}

		HFloat reci = 1.0f / mCopy[iy][iy];
		mCopy.v[iy].v[iy] = 1.0f;
		for (int j = 0; j < 4; j++) mCopy.v[iy].v[j] *= reci;

		for (int j = 0; j < 4; j++)
		{
			if (j != iy)
			{
				HFloat save = mCopy.v[j].v[iy];
				mCopy.v[j].v[iy] = 0.0f;
				for (int k = 0; k < 4; k++)
					mCopy.v[j].v[k] -= mCopy.v[iy].v[k] * save;
			}
		}
	}

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			result.v[i].v[sq[j]] = mCopy[i][j];
	return result;
}

HFloat4x4 HFloat4x4::SetLookAtLH(const HFloat3 & eyePos, const HFloat3 & focusPos, const HFloat3 & upDir) 
{
	return SetLookToLH(eyePos, focusPos - eyePos, upDir);
}

HFloat4x4 HFloat4x4::SetLookToLH(const HFloat3 & eyePos, const HFloat3 & eyeDir, const HFloat3 & upDir) 
{
	HFloat3 vZ = eyeDir.Normalize();
	HFloat3 vX = upDir.Cross(vZ).Normalize();
	HFloat3 vY = vZ.Cross(vX);

	return Set(
		vX.x,	vX.y,	vX.z,	-vX.Dot(eyePos),
		vY.x,	vY.y,	vY.z,	-vY.Dot(eyePos),
		vZ.x,	vZ.y,	vZ.z,	-vZ.Dot(eyePos),
		0.0f,	0.0f,	0.0f,	1.0f
	);
}

HFloat4x4 HFloat4x4::SetOrthoLH(const HFloat width, const HFloat height, const HFloat zNear, const HFloat zFar)
{
	HFloat fRange = 1.0f / (zFar - zNear);
	return Set(
		2 / width, 0.0f, 0.0f, 0.0f,
		0.0f, 2 / height, 0.0f, 0.0f,
		0.0f, 0.0f, fRange, -fRange * zNear,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

HFloat4x4 HFloat4x4::SetPerspLH(const HFloat width, const HFloat height, const HFloat zNear, const HFloat zFar)
{
	HFloat zNear2 = zNear + zNear;
	HFloat fRange = zFar / (zFar - zNear);
	return Set(
		zNear2 / width, 0.0f, 0.0f, 0.0f,
		0.0f, zNear2 / height, 0.0f, 0.0f,
		0.0f, 0.0f, fRange, -fRange * zNear,
		0.0f, 0.0f, 1.0f, 0.0f
	);
}

HFloat4x4 HFloat4x4::SetPerspFovLH(const float fovY, const float aspectRatio, const float zNear, const float zFar)
{
	HFloat halfFov = 0.5f * fovY;
	HFloat height = cosf(halfFov) / sinf(halfFov);
	HFloat width = height / aspectRatio;
	HFloat fRange = zFar / (zFar - zNear);
	return Set(
		width, 0.0f, 0.0f, 0.0f,
		0.0f, height, 0.0f, 0.0f,
		0.0f, 0.0f, fRange, -fRange * zNear,
		0.0f, 0.0f, 1.0f, 0.0f
	);
}

HFloat3 HFloat4x4::GetEulerXYZ()
{
	HFloat3 result;
	if (_13 < 1.0f)
	{
		if (_13 > -1.0f)
		{
			result.y = asinf(_13);
			result.x = atan2f(-_23, _33);
			result.z = atan2f(-_12, _11);
		}
		else
		{
			result.y = -H_PIDIV2;
			result.x = -atan2f(_21, _22);
			result.z = 0.0f;
		}
	}
	else
	{
		result.y = H_PIDIV2;
		result.x = atan2f(_21, _22);
		result.z = 0.0f;
	}
	return result;
}

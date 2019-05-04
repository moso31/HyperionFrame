#include "HMatrix.h"
#include "HQuaternion.h"

HMatrix4x4::HMatrix4x4(HFloat _00, HFloat _01, HFloat _02, HFloat _03, HFloat _10, HFloat _11, HFloat _12, HFloat _13, HFloat _20, HFloat _21, HFloat _22, HFloat _23, HFloat _30, HFloat _31, HFloat _32, HFloat _33) :
	_00(_00), _01(_01), _02(_02), _03(_03),
	_10(_10), _11(_11), _12(_12), _13(_13),
	_20(_20), _21(_21), _22(_22), _23(_23),
	_30(_30), _31(_31), _32(_32), _33(_33)
{
}

HMatrix4x4::HMatrix4x4(const HMatrix4x4 & m) :
	_00(m._00), _01(m._01), _02(m._02), _03(m._03),
	_10(m._10), _11(m._11), _12(m._12), _13(m._13),
	_20(m._20), _21(m._21), _22(m._22), _23(m._23),
	_30(m._30), _31(m._31), _32(m._32), _33(m._33)
{
}

HMatrix4x4 HMatrix4x4::operator+(const HMatrix4x4 & m) const
{
	HMatrix4x4 r;
	for (int i = 0; i < 4; i++)
		r[i] = v[i] + m.v[i];
	return r;
}

HMatrix4x4 HMatrix4x4::operator-(const HMatrix4x4 & m) const
{
	HMatrix4x4 r;
	for (int i = 0; i < 4; i++)
		r[i] = v[i] - m.v[i];
	return r;
}

HMatrix4x4 HMatrix4x4::operator*(const HMatrix4x4 & m) const
{
	HMatrix4x4 r;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				r.v[i].v[j] += v[i].v[k] + m.v[k].v[j];
			}
		}
	}
	return r;
}

HMatrix4x4 HMatrix4x4::SetIdentity()
{
	_00 = 1.0f; _01 = 0.0f; _02 = 0.0f; _03 = 0.0f;
	_10 = 0.0f; _11 = 1.0f; _12 = 0.0f; _13 = 0.0f;
	_20 = 0.0f; _21 = 0.0f; _22 = 1.0f; _23 = 0.0f;
	_30 = 0.0f; _31 = 0.0f; _32 = 0.0f; _33 = 1.0f;
}

HMatrix4x4 HMatrix4x4::SetZero()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			v[i].v[j] = 0.0f;
	}
}

HMatrix4x4 HMatrix4x4::SetNaN()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			v[i].v[j] = NAN;
	}
}

HMatrix4x4 HMatrix4x4::SetTranslation(HFloat x, HFloat y, HFloat z)
{
	return HMatrix4x4(1.0f, 0.0f, 0.0f, x,
		0.0f, 1.0f, 0.0f, y,
		0.0f, 0.0f, 1.0f, z,
		0.0f, 0.0f, 0.0f, 1.0f);
}

HMatrix4x4 HMatrix4x4::SetRotationAxis(const HVector3 & axis, HFloat angle)
{
	return SetRotationNormal(axis.Normalize(), angle);
}

HMatrix4x4 HMatrix4x4::SetRotationNormal(const HVector3 & normal, HFloat angle)
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

	return HMatrix4x4(c + normal.x * normal.x * t, t1 - t2, t3 + t4, 0.0f,
		t1 + t2, c + normal.y * normal.y * t, t5 - t6, 0.0f,
		t3 - t4, t5 + t6, c + normal.z * normal.z * t, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

HMatrix4x4 HMatrix4x4::SetRotationQuaternion(const HQuaternion & q)
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

	return HMatrix4x4((sqx - sqy - sqz + sqw) * invs, 2.0 * (t1 - t2) * invs, 2.0 * (t3 + t4) * invs, 0.0f,
		2.0 * (t1 + t2) * invs, (-sqx + sqy - sqz + sqw) * invs, 2.0 * (t5 - t6) * invs, 0.0f,
		2.0 * (t3 - t4) * invs, 2.0 * (t5 + t6) * invs, (-sqx - sqy + sqz + sqw) * invs, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

HMatrix4x4 HMatrix4x4::SetRotationXYZ(HFloat pitch, HFloat yaw, HFloat roll)
{
	float cx = cosf(pitch);
	float cy = cosf(yaw);
	float cz = cosf(roll);
	float sx = sinf(pitch);
	float sy = sinf(yaw);
	float sz = sinf(roll);
	float cxcz = cx * cz;
	float sxsz = sx * sz;
	float sxcz = sx * cz;
	return HMatrix4x4(cy * cz, cy * sz, -sy, 0.0f,
		sxcz * sy - cx * sz, sxsz * sy + cxcz, sx * cy, 0.0f,
		cxcz * sy + sxsz, cx * sy * sz - sxcz, cx * cy, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

HMatrix4x4 HMatrix4x4::SetScale(HFloat x, HFloat y, HFloat z)
{
	return HMatrix4x4(x, 0.0f, 0.0f, 0.0f,
		0.0f, y, 0.0f, 0.0f,
		0.0f, 0.0f, z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

HMatrix4x4 HMatrix4x4::Transpose() const
{
	return HMatrix4x4(_00, _10, _20, _30,
		_01, _11, _21, _31,
		_02, _12, _22, _32,
		_03, _13, _23, _33);
}

HMatrix4x4 HMatrix4x4::Inverse() const
{
	int b[4] = { 0, 0, 0, 0 };
	int sq[4] = { 0, 1, 2, 3 };
	HMatrix4x4 mCopy(*this);
	HMatrix4x4 result;
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
					HFloat val = abs(mCopy[j][k]);
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

HMatrix4x4 HMatrix4x4::LookAt(const HVector3 & eyePos, const HVector3 & focusPos, const HVector3 & upDir) const
{
	return LookTo(eyePos, focusPos - eyePos, upDir);
}

HMatrix4x4 HMatrix4x4::LookTo(const HVector3 & eyePos, const HVector3 & eyeDir, const HVector3 & upDir) const
{
	HVector3 dir = eyeDir.Normalize();
	HVector3 left = upDir.Cross(dir).Normalize();
	HVector3 up = dir.Cross(left);

	return HMatrix4x4(
		left.x, up.x, dir.x, 0.0f,
		left.y, up.y, dir.y, 0.0f,
		left.z, up.z, dir.z, 0.0f,
		-left.Dot(eyePos), -up.Dot(eyePos), -dir.Dot(eyePos), 1.0f
	);
}

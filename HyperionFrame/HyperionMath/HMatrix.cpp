#include "HMatrix.h"
#include "HQuaternion.h"

HMatrix4x4::HMatrix4x4(HFloat _00, HFloat _01, HFloat _02, HFloat _03, HFloat _10, HFloat _11, HFloat _12, HFloat _13, HFloat _20, HFloat _21, HFloat _22, HFloat _23, HFloat _30, HFloat _31, HFloat _32, HFloat _33) :
	_00(_00), _01(_01), _02(_02), _03(_03),
	_10(_10), _11(_11), _12(_12), _13(_13),
	_20(_20), _21(_21), _22(_22), _23(_23),
	_30(_30), _31(_31), _32(_32), _33(_33)
{
}

HMatrix4x4 HMatrix4x4::SetIdentity()
{
	_00 = 1.0f; _01 = 0.0f; _02 = 0.0f; _03 = 0.0f;
	_10 = 0.0f; _11 = 1.0f; _12 = 0.0f; _13 = 0.0f;
	_20 = 0.0f; _21 = 0.0f; _22 = 1.0f; _23 = 0.0f;
	_30 = 0.0f; _31 = 0.0f; _32 = 0.0f; _33 = 1.0f;
}

HMatrix4x4 HMatrix4x4::SetTranslation(HFloat x, HFloat y, HFloat z)
{
	return HMatrix4x4(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 
		0.0f, 0.0f, 1.0f, 0.0f,
		x, y, z, 1.0f);
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

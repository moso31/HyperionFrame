#include "HQuaternion.h"
#include "HMatrix.h"

HQuaternion::HQuaternion(HFloat3 axis, HFloat angle)
{
	HFloat fHalfAngle = angle * 0.5f;
	HFloat c = cosf(fHalfAngle);
	HFloat s = sinf(fHalfAngle);
	x = axis.x * s;
	y = axis.y * s;
	z = axis.z * s;
	w = c;
}

HFloat4x4 HQuaternion::ToMatrix()
{
	HFloat xx = x * x;
	HFloat yy = y * y;
	HFloat zz = z * z;
	HFloat xy = x * y;
	HFloat xz = x * z;
	HFloat xw = x * w;
	HFloat yz = y * z;
	HFloat yw = y * w;
	HFloat zw = z * w;
	return HFloat4x4(1.0f - 2.0f * yy - 2.0f * zz, 2.0f * xy - 2.0f * zw, 2.0f * xz + 2.0f * yw, 0.0f,
		2.0f * xy + 2.0f * zw, 1.0f - 2.0f * xx - 2.0f * zz, 2.0f * yz - 2.0f * xw, 0.0f,
		2.0f * xz - 2.0f * yw, 2.0f * yz + 2.0f * xw, 1.0f - 2.0f * xx - 2.0f * yy, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

HFloat3 HQuaternion::ToEulerXYZ()
{
	HFloat xx = x * x;
	HFloat yy = y * y;
	HFloat zz = z * z;
	HFloat ww = w * w;

	//HFloat x1 = -2.0f * (x * y - w * z);
	//HFloat x2 = ww + xx - yy - zz;
	//HFloat y1 = 2.0f * (x * z + w * y);
	//HFloat z1 = -2.0f * (y * z - w * x);
	//HFloat z2 = ww - xx - yy + zz;

	HFloat z1 = x * y + w * z;
	HFloat z2 = ww + xx - yy - zz;
	HFloat y1 = -2.0f * (x * z - w * y);
	HFloat x1 = 2.0f * (y * z + w * x);
	HFloat x2 = ww - xx - yy + zz;

	return HFloat3(atan2f(x1, x2), asinf(y1), atan2f(z1, z2));
}

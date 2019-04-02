#pragma once
#include "HMathType.h"
#include "HVector.h"

class HMatrix4x4
{
public:
	HMatrix4x4() {}
	HMatrix4x4(HFloat _00, HFloat _01, HFloat _02, HFloat _03, HFloat _10, HFloat _11, HFloat _12, HFloat _13, HFloat _20, HFloat _21, HFloat _22, HFloat _23, HFloat _30, HFloat _31, HFloat _32, HFloat _33);
	~HMatrix4x4() {}

	HMatrix4x4 SetIdentity();
	HMatrix4x4 SetTranslation(HFloat x, HFloat y, HFloat z);
	HMatrix4x4 SetRotationAxis(const HVector3& axis, HFloat angle);
	HMatrix4x4 SetRotationNormal(const HVector3& normal, HFloat angle);
	HMatrix4x4 SetRotationQuaternion(const HQuaternion& q);
	HMatrix4x4 SetRotationXYZ(HFloat pitch, HFloat yaw, HFloat roll);
	HMatrix4x4 SetScale(HFloat x, HFloat y, HFloat z);

public:
	union
	{
		struct { HFloat _00, _01, _02, _03, _10, _11, _12, _13, _20, _21, _22, _23, _30, _31, _32, _33; };
		HVector4 v[4];
	};
};

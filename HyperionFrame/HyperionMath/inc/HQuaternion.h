#pragma once
#include "HMathType.h"

class HQuaternion
{
public:
	HQuaternion() = default;
	HQuaternion(HFloat x, HFloat y, HFloat z, HFloat w) : x(x), y(y), z(z), w(w) {}
	HQuaternion(HFloat3 axis, HFloat angle);
	~HQuaternion() {}

	HFloat4x4 ToMatrix();
	HFloat3 ToEulerXYZ();

public:
	HFloat x, y, z, w;
};


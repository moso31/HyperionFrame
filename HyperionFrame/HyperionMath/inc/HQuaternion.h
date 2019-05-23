#pragma once
#include "HMathType.h"

class HQuaternion
{
public:
	HQuaternion() = default;
	HQuaternion(HFloat x, HFloat y, HFloat z, HFloat w) : x(x), y(y), z(z), w(w) {}
	~HQuaternion() {}

public:
	HFloat x, y, z, w;
};


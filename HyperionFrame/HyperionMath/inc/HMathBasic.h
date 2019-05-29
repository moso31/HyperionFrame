#pragma once
#include <math.h>
#include <algorithm>
#include "HMathType.h"

using namespace std;

inline HFloat Clamp(HFloat value, HFloat left, HFloat right)
{
	if (value < left) return left;
	if (value > right) return right;
	return value;
}

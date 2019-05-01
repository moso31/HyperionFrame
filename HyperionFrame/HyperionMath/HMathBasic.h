#pragma once
#include <math.h>
#include "HMathType.h"

using namespace std;

HFloat Clamp(HFloat value, HFloat left, HFloat right)
{
	if (value < left) return left;
	if (value > right) return right;
	return value;
}

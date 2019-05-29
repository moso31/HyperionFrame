#pragma once
#include <math.h>
#include <algorithm>
#include "HMathType.h"

using namespace std;

static const HFloat H_MACHINEEPSLION = numeric_limits<HFloat>::epsilon() * 0.5f;

template <typename T, typename U, typename V>
inline T Clamp(T val, U low, V high) {
	if ((T)val < (T)low) return (T)low;
	else if ((T)val > (T)high) return (T)high;
	else return (T)val;
}

inline HFloat gamma(HInt n)
{
	return (n * H_MACHINEEPSLION) / (1 - n * H_MACHINEEPSLION);
}
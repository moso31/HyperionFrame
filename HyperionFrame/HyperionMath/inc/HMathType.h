#pragma once
#include <limits>

typedef char HChar;
typedef unsigned char HByte;
typedef short HShort;
typedef unsigned short HUShort;
typedef int HInt;
typedef __int64 HInt64;
typedef unsigned int HUInt;
typedef unsigned __int64 HUInt64;
typedef long HLong;
typedef unsigned long HULong;
typedef float HFloat;
typedef double HDouble;

class HFloat2;
class HFloat3;
class HFloat4;
class HQuaternion;
class HFloat4x4;

const HFloat H_PI		= 3.14159265358979323846f;
const HFloat H_2PI		= 6.28318530717958647692f;
const HFloat H_4PI		= 12.5663706143591729538f;
const HFloat H_PIDIV2	= 1.57079632679489661923f;
const HFloat H_PIDIV4	= 0.78539816339744830961f;
const HFloat H_1DIVPI	= 0.31830988618379067154f;
const HFloat H_1DIV2PI	= 0.15915494309189533577f;
const HFloat H_1DIV4PI	= 0.07957747154594766788f;

const HFloat H_INFINITYF = std::numeric_limits<HFloat>::infinity();
const HFloat H_NANF = std::numeric_limits<HFloat>::quiet_NaN();
const HDouble H_INFINITY = std::numeric_limits<HDouble>::infinity();
const HDouble H_NAN = std::numeric_limits<HDouble>::quiet_NaN();

const HFloat H_EPSILON = 1e-6f;
const HFloat H_ONE_MINUS_EPSILON = 0.9999994f;
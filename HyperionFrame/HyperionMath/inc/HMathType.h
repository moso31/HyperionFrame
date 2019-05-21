#pragma once

typedef char HChar;
typedef unsigned char HByte;
typedef short HShort;
typedef unsigned short HUShort;
typedef int HInt;
typedef __int64 HInt64;
typedef unsigned int HUInt;
typedef unsigned __int64 HUInt64;
typedef float HFloat;
typedef double HDouble;

class HFloat2;
class HFloat3;
class HFloat4;
class HQuaternion;
class HMatrix4x4;

const float H_PI		= 3.14159265358979323846f;
const float H_2PI		= 6.28318530717958647692f;
const float H_4PI		= 12.5663706143591729538f;
const float H_PIDIV2	= 1.57079632679489661923f;
const float H_PIDIV4	= 0.78539816339744830961f;
const float H_1DIVPI	= 0.31830988618379067154f;
const float H_1DIV2PI	= 0.15915494309189533577f;
const float H_1DIV4PI	= 0.07957747154594766788f;

const float H_EPSILON = 1e-6f;
const float H_ONE_MINUS_EPSILON = 0.9999994f;
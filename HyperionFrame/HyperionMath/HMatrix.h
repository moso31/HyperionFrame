#pragma once
#include "HMathType.h"
#include "HVector.h"

void f()
{
	DirectX::XMMatrixIdentity
}

class HMatrix4x4
{
public:
	HMatrix4x4();
	HMatrix4x4(HFloat _00, HFloat _01, HFloat _02, HFloat _03, HFloat _10, HFloat _11, HFloat _12, HFloat _13, HFloat _20, HFloat _21, HFloat _22, HFloat _23, HFloat _30, HFloat _31, HFloat _32, HFloat _33);
	~HMatrix4x4() {}

	static HMatrix4x4 Identity() { return HMatrix4x4(); }

public:
	union
	{
		struct { HFloat _00, _01, _02, _03, _10, _11, _12, _13, _20, _21, _22, _23, _30, _31, _32, _33; };
		HVector4 v[4];
	};
};

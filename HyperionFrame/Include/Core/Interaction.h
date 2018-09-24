#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class SurfaceInteraction
{
public:
	SurfaceInteraction();
	~SurfaceInteraction();

public:
	XMFLOAT3 n;
	XMFLOAT3 wo;
};

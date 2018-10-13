#pragma once
#include <map>
#include <list>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <functional>

#include <Windows.h>
#include <wincodec.h>
#include <wrl.h>
#include <time.h>

#include <d3d12.h>
#include <dxgi1_4.h>
#include <dwrite.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

#include <pix.h>

#if _DEBUG
#include <dxgidebug.h>
#endif

using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;

#include "d3dx12.h"

class SurfaceInteraction;
class Shape;
class HMaterial;
class VisibilityTester;
class BSDF;
class BxDF;

typedef XMFLOAT3 XMCOLOR3;
typedef XMFLOAT4 XMCOLOR4;
typedef XMVECTOR XMCOLORV;

static const float H_PI			= 3.14159265358979323846f;
static const float H_2PI		= 6.28318530717958647692f;
static const float H_4PI		= 12.5663706143591729538f;
static const float H_PIDIV2		= 1.57079632679489661923f;
static const float H_PIDIV4		= 0.78539816339744830961f;
static const float H_1DIVPI		= 0.31830988618379067154f;
static const float H_1DIV2PI	= 0.15915494309189533577f;
static const float H_1DIV4PI	= 0.07957747154594766788f;
static const float H_SQRT2		= 1.41421356237309504880f;
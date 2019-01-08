#pragma once
#include <Windows.h>
#include <wincodec.h>
#include <wrl.h>
#include <time.h>
#include <io.h>

#include <map>
#include <list>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <functional>
#include <thread>

#include <d3d12.h>
#include <dxgi1_4.h>
#include <dwrite.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

#include <pix.h>
#include "d3dx12.h"

#if _DEBUG
#include <dxgidebug.h>
#endif

using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;

class SurfaceInteraction;
class VisibilityTester;
class BSDF;
class BxDF;
class HScene;
class HBVHTree;

class Shape;
class Transform;
class Camera;
class Box;
class Sphere;
class HMesh;

class HMaterial;
class HMatteMaterial;
class HGlassMaterial;
class HMirrorMaterial;

class HLight;
class HPointLight;

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

static const float H_RADTODEG	= 180.0f / H_PI;
static const float H_DEGTORAD	= H_PI / 180.0f;

#define H_EPSILON 1e-6f
#define ONE_MINUS_EPSILON 0.9999994f
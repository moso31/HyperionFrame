#pragma once
#include <Windows.h>
#include <WinUser.h>
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
#include <DirectXColors.h>

#include <pix.h>
#include "d3dx12.h"

#if _DEBUG
#include <dxgidebug.h>
#endif

#include "HyperionMath.h"

using namespace std;
using namespace Microsoft::WRL;

struct HEventArg
{
	HUShort X, Y;
	union
	{
		HUShort VKey;
		HUShort VMouse;
	};
	LONG LastX, LastY;
	HUShort VWheel;
};

class HListener;

class SurfaceInteraction;
class VisibilityTester;
class BSDF;
class BxDF;
class HScene;
class HBVHTree;

class HShape;
class HObject;
class Transform;
class Camera;
class Box;
class Sphere;
class HMesh;
class HSegment;

enum HSCRIPTTYPE;
class HScript;

class HMaterial;
class HMatteMaterial;
class HGlassMaterial;
class HMirrorMaterial;

class HLight;
class HPointLight;

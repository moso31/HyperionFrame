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
class Material;
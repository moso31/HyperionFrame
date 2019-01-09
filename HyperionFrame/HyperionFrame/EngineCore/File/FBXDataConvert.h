#pragma once
#include <fbxsdk.h>
#include <DirectXMath.h>

using namespace DirectX;

XMFLOAT2 ReadXMFLOAT2(FbxVector2 pValue);
XMFLOAT3 ReadXMFLOAT3(FbxVector4 pValue);
XMFLOAT3 ReadXMFLOAT3(FbxColor pValue);
XMFLOAT4 ReadXMFLOAT4(FbxVector4 pValue);
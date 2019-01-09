#include "FBXDataConvert.h"

XMFLOAT2 ReadXMFLOAT2(FbxVector2 pValue)
{
	XMFLOAT2 result;
	result.x = (float)pValue[0];
	result.y = (float)pValue[1];
	return result;
}

XMFLOAT3 ReadXMFLOAT3(FbxVector4 pValue)
{
	XMFLOAT3 result;
	result.x = (float)pValue[0];
	result.y = (float)pValue[1];
	result.z = (float)pValue[2];
	return result;
}

XMFLOAT3 ReadXMFLOAT3(FbxColor pValue)
{
	return XMFLOAT3((float)pValue.mRed, (float)pValue.mGreen, (float)pValue.mBlue);
}

XMFLOAT4 ReadXMFLOAT4(FbxVector4 pValue)
{
	XMFLOAT4 result;
	result.x = (float)pValue[0];
	result.y = (float)pValue[1];
	result.z = (float)pValue[2];
	result.w = (float)pValue[3];
	return result;
}

#include "FBXDataConvert.h"

HFloat2 ReadHFLOAT2(FbxVector2 pValue)
{
	HFloat2 result;
	result.x = (float)pValue[0];
	result.y = (float)pValue[1];
	return result;
}

HFloat3 ReadHFLOAT3(FbxVector4 pValue)
{
	HFloat3 result;
	result.x = (float)pValue[0];
	result.y = (float)pValue[1];
	result.z = (float)pValue[2];
	return result;
}

HFloat3 ReadHFLOAT3(FbxColor pValue)
{
	return HFloat3((float)pValue.mRed, (float)pValue.mGreen, (float)pValue.mBlue);
}

HFloat4 ReadHFLOAT4(FbxVector4 pValue)
{
	HFloat4 result;
	result.x = (float)pValue[0];
	result.y = (float)pValue[1];
	result.z = (float)pValue[2];
	result.w = (float)pValue[3];
	return result;
}

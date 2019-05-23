#include "FBXDataConvert.h"

HFloat2 ReadHFLOAT2(FbxVector2 pValue)
{
	HFloat2 result;
	result.x = (HFloat)pValue[0];
	result.y = (HFloat)pValue[1];
	return result;
}

HFloat3 ReadHFLOAT3(FbxVector4 pValue)
{
	HFloat3 result;
	result.x = (HFloat)pValue[0];
	result.y = (HFloat)pValue[1];
	result.z = (HFloat)pValue[2];
	return result;
}

HFloat3 ReadHFLOAT3(FbxColor pValue)
{
	return HFloat3((HFloat)pValue.mRed, (HFloat)pValue.mGreen, (HFloat)pValue.mBlue);
}

HFloat4 ReadHFLOAT4(FbxVector4 pValue)
{
	HFloat4 result;
	result.x = (HFloat)pValue[0];
	result.y = (HFloat)pValue[1];
	result.z = (HFloat)pValue[2];
	result.w = (HFloat)pValue[3];
	return result;
}

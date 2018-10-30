#pragma once
#include "HSampler.h"

class HPixelSampler : public HSampler
{
public:
	HPixelSampler(int64_t samplesPerPixel, int nSampledDimensions);
	bool NextSample();
	bool GotoSample(int64_t sampleIndex);
	float Get1D();
	XMFLOAT2 Get2D();

protected:
	std::vector<std::vector<float>> samples1D;
	std::vector<std::vector<XMFLOAT2>> samples2D;
	int current1DDimension = 0, current2DDimension = 0;
};

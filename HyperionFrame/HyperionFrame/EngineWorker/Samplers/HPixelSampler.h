#pragma once
#include "HSampler.h"

class HPixelSampler : public HSampler
{
public:
	HPixelSampler(int64_t samplesPerPixel, HInt nSampledDimensions);
	bool NextSample();
	bool GotoSample(int64_t sampleIndex);
	HFloat Get1D();
	HFloat2 Get2D();

protected:
	std::vector<std::vector<HFloat>> samples1D;
	std::vector<std::vector<HFloat2>> samples2D;
	HInt current1DDimension = 0, current2DDimension = 0;
};

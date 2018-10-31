#pragma once
#include "HPixelSampler.h"

class HDefaultSampler : public HPixelSampler
{
public:
	HDefaultSampler(int xPixelSamples, int yPixelSamples, bool jitterSamples, int nSampledDimensions);

	void GenerateSampleData(XMINT2& pixel);
	unique_ptr<HSampler> Clone(int seed = 0);

private:
	const int xPixelSamples, yPixelSamples;
	const bool jitterSamples;
};

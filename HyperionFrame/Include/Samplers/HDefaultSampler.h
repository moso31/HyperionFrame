#pragma once
#include "HPixelSampler.h"

class HDefaultSampler : public HPixelSampler
{
public:
	HDefaultSampler(int xPixelSamples, int yPixelSamples, bool jitterSamples, int nSampledDimensions) : 
		HPixelSampler(xPixelSamples * yPixelSamples, nSampledDimensions),
		xPixelSamples(xPixelSamples),
		yPixelSamples(yPixelSamples),
		jitterSamples(jitterSamples) {}

	void GenerateSampleData(XMINT2& pixel);

private:
	const int xPixelSamples, yPixelSamples;
	const bool jitterSamples;
};

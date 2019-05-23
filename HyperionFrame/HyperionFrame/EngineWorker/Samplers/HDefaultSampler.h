#pragma once
#include "HPixelSampler.h"

class HDefaultSampler : public HPixelSampler
{
public:
	HDefaultSampler(HInt xPixelSamples, HInt yPixelSamples, bool jitterSamples, HInt nSampledDimensions);

	void GenerateSampleData(HInt2& pixel);
	unique_ptr<HSampler> Clone(HInt seed = 0);

private:
	const HInt xPixelSamples, yPixelSamples;
	const bool jitterSamples;
};

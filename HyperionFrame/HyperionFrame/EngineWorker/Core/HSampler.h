#pragma once
#include "header.h"

class HSampler
{
public:
	virtual ~HSampler() {}
	HSampler(HInt64 samplesPerPixel);

	virtual void GenerateSampleData(HInt2& pixel);
	virtual bool NextSample();
	virtual bool GotoSample(HInt64 sampleNum);
	virtual float Get1D() = 0;
	virtual HFloat2 Get2D() = 0;

	virtual unique_ptr<HSampler> Clone(int seed) = 0;

	const HInt64 samplesPerPixel;

protected:
	HInt2 currentPixel;
	HInt64 currentPixelSampleIndex;
};
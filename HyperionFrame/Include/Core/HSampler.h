#pragma once
#include "header.h"

class HSampler
{
public:
	virtual ~HSampler() {}
	HSampler(int64_t samplesPerPixel);

	virtual void GenerateSampleData(XMINT2& pixel);
	virtual bool NextSample();
	virtual bool GotoSample(int64_t sampleNum);
	virtual float Get1D() = 0;
	virtual XMFLOAT2 Get2D() = 0;

	virtual unique_ptr<HSampler> Clone(int seed) = 0;

	const int64_t samplesPerPixel;

protected:
	XMINT2 currentPixel;
	int64_t currentPixelSampleIndex;
};
#include "HSampler.h"

void HSampler::GenerateSampleData(XMINT2 & pixel)
{
	currentPixel = pixel;
	currentPixelSampleIndex = 0;
}

bool HSampler::NextSample()
{
	currentPixelSampleIndex++;
	return currentPixelSampleIndex < samplesPerPixel;
}

bool HSampler::GotoSample(int64_t sampleIndex)
{
	currentPixelSampleIndex = sampleIndex;
	return currentPixelSampleIndex < samplesPerPixel;
}

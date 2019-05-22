#include "HSampler.h"

HSampler::HSampler(HInt64 samplesPerPixel) :
	samplesPerPixel(samplesPerPixel)
{
}

void HSampler::GenerateSampleData(HInt2 & pixel)
{
	currentPixel = pixel;
	currentPixelSampleIndex = 0;
}

bool HSampler::NextSample()
{
	currentPixelSampleIndex++;
	return currentPixelSampleIndex < samplesPerPixel;
}

bool HSampler::GotoSample(HInt64 sampleIndex)
{
	currentPixelSampleIndex = sampleIndex;
	return currentPixelSampleIndex < samplesPerPixel;
}

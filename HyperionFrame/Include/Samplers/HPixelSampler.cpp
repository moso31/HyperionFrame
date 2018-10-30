#include "HPixelSampler.h"

HPixelSampler::HPixelSampler(int64_t samplesPerPixel, int nSampledDimensions) :
	HSampler(samplesPerPixel)
{
	samples1D.resize(nSampledDimensions);
	samples2D.resize(nSampledDimensions);
}

bool HPixelSampler::NextSample()
{
	current1DDimension = current2DDimension = 0;
	return HSampler::NextSample();
}

bool HPixelSampler::GotoSample(int64_t sampleIndex)
{
	current1DDimension = current2DDimension = 0;
	return HSampler::GotoSample(sampleIndex);
}

float HPixelSampler::Get1D()
{
	if (current1DDimension < samples1D.size())
		return samples1D[current1DDimension++][currentPixelSampleIndex];
	else return rng.UniformFloat();
}

XMFLOAT2 HPixelSampler::Get2D()
{
	if (current2DDimension < samples2D.size())
		return samples2D[current2DDimension++][currentPixelSampleIndex];
	else return rng.UniformFloat();
}
